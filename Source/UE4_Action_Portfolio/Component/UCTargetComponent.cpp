#include "Component/UCTargetComponent.h"
#include "Global.h" 
#include "GameFramework/Character.h"
#include "CStateComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

UUCTargetComponent::UUCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CHelpers::GetAsset<UParticleSystem>(&Particle, "ParticleSystem'/Game/rpgeffects/Particles/P_Targeting_Player_Select.P_Targeting_Player_Select'");
}

void UUCTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner()); 
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void UUCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckNull(Target); 

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(Target); 

	bool b = false;
	b |= state->IsDeadMode(); 
	b |= Target->GetDistanceTo(OwnerCharacter) >= TraceRadius;
	
	if (b == true)
	{
		EndTargeting(); 

		return; 
	}

	// Targeting이 되었을 때 Target이 화면의 일정 범위를 벗어났을 경우 Target이 화면의 일정 범위 안에 존재하도록 카메라를 조정합니다.
	if (IsInViewport(GetScreenPositionOfActor(Target).Get<0>(), 0.2f) == false)
	{
		const FRotator CurrentRotator = PlayerController->GetControlRotation(); 
		const FRotator TargetRotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), Target->GetActorLocation()); 
		const FRotator InterpRotator = UKismetMathLibrary::RInterpTo(CurrentRotator, TargetRotator, DeltaTime, 1.0f); 

		PlayerController->SetControlRotation(InterpRotator); 
	}
}

void UUCTargetComponent::ToggleTarget()
{
	if (!!Target)
	{
		EndTargeting();

		return;
	}

	StartTargeting();
}

void UUCTargetComponent::ChangeTargetLeft()
{
	ChangeTarget(false); 
}

void UUCTargetComponent::ChangeTargetRight()
{
	ChangeTarget(true); 
}

void UUCTargetComponent::StartTargeting()
{
	SetTraceTargets(); 
	SetTarget(); 
}

void UUCTargetComponent::EndTargeting()
{
	Target = NULL; 
	TraceTargets.Empty(); 

	if (!!Attached)
		Attached->DestroyComponent(); 
}

void UUCTargetComponent::SetTraceTargets()
{
	FVector start = OwnerCharacter->GetActorLocation();
	FVector end = FVector(start.X, start.Y, start.Z + 1);

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	TArray<FHitResult> hitResults;

	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, TraceRadius, "Pawn", false, ignoreActors, Debug, hitResults, true, FLinearColor::Green, FLinearColor::Red, 1.0f);

	for (const FHitResult& result : hitResults)
	{
		/*if (result.GetActor()->GetClass() == OwnerCharacter->GetClass())
		{
			continue;
		}
		*/

		ACharacter* character = Cast<ACharacter>(result.GetActor());

		if (!!character)
			TraceTargets.AddUnique(character);
	}
}

void UUCTargetComponent::SetTarget()
{
	float angle = -2.0f;
	ACharacter* target = NULL;

	for (ACharacter* character : TraceTargets)
	{
		FVector direction = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
		FVector offset = character->GetActorLocation() - OwnerCharacter->GetActorLocation();
		offset = offset.GetSafeNormal();

		float temp = direction | offset;
		if (temp < angle)
			continue;

		angle = temp;
		target = character;
	}

	ChangeCursor(target);
}

void UUCTargetComponent::ChangeTarget(bool InRight)
{
	CheckNull(Target);

	TMap<float, ACharacter*> map;
	for (ACharacter* character : TraceTargets)
	{
		if (Target == character)
			continue;

		FVector targetLocation = character->GetActorLocation();
		FVector OwnerLocation = OwnerCharacter->GetActorLocation();
		FVector OwnerToTarget = targetLocation - OwnerLocation;

		FQuat quat = FQuat(OwnerCharacter->GetControlRotation());
		FVector forward = quat.GetForwardVector();
		FVector up = quat.GetUpVector();

		FVector cross = forward ^ OwnerToTarget;
		float dot = cross | up;

		map.Add(dot, character);
	}

	float minimum = FLT_MAX;
	ACharacter* target = NULL;

	TArray<float> Keys;
	map.GetKeys(Keys);
	for (float key : Keys)
	{
		if (InRight == true)
		{
			if (key < 0.0f)
				continue;
		}
		else
		{
			if (key > 0.0f)
				continue;
		}

		if (FMath::Abs(key) > minimum)
			continue;

		minimum = FMath::Abs(key);
		target = *map.Find(key);
	}

	ChangeCursor(target);
}

void UUCTargetComponent::ChangeCursor(ACharacter* InTarget)
{
	if (!!InTarget)
	{
		if (!!Attached)
			Attached->DestroyComponent(); 

		Attached = UGameplayStatics::SpawnEmitterAttached(Particle, InTarget->GetMesh(), "root"); 
		Target = InTarget; 

		return; 
	}

	EndTargeting();
}

bool UUCTargetComponent::IsInViewport(FVector2D ActorScreenPosition, float ScreenRatio) const
{
	FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();

	//화면 전체를 사용할 경우
	if (ScreenRatio == 0.0f || UKismetMathLibrary::Abs(ScreenRatio) > 1.0f || (ScreenRatio == (1.0f - ScreenRatio)))
	{
		return ActorScreenPosition.X >= 0.0f && ActorScreenPosition.Y >= 0.0f
			&& ActorScreenPosition.X <= ViewportSize.X && ActorScreenPosition.Y <= ViewportSize.Y;
	}

	float LargeScreenRatio;
	float SmallScreenRatio;

	if (ScreenRatio < (1.0f - ScreenRatio))
	{
		LargeScreenRatio = 1.0f - ScreenRatio;
		SmallScreenRatio = ScreenRatio;
	}
	else
	{
		LargeScreenRatio = ScreenRatio;
		SmallScreenRatio = 1.0f - ScreenRatio;
	}

	return ActorScreenPosition.X >= ViewportSize.X * SmallScreenRatio && ActorScreenPosition.X <= ViewportSize.X * LargeScreenRatio
		&& ActorScreenPosition.Y >= ViewportSize.Y * SmallScreenRatio && ActorScreenPosition.Y <= ViewportSize.Y * LargeScreenRatio;
}

TTuple<FVector2D, bool> UUCTargetComponent::GetScreenPositionOfActor(AActor* SearchActor) const
{
	FVector2D ScreenLocation = FVector2D::ZeroVector;
	bool bResult = UGameplayStatics::ProjectWorldToScreen(PlayerController, SearchActor->GetActorLocation(), ScreenLocation);

	return MakeTuple(ScreenLocation, bResult);
}


