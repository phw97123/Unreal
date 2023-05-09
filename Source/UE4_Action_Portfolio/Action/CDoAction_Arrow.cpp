#include "CDoAction_Arrow.h"
#include "Global.h"
#include "CAim.h"
#include "CArrow.h"	
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "Component/CStatusComponent.h"
#include "CWeapon_Interface.h"
#include "Engine/StaticMeshActor.h"
#include "Components/SkeletalMeshComponent.h"

void ACDoAction_Arrow::BeginPlay()
{
	Super::BeginPlay(); 

	Aim = NewObject<UCAim>(); 
	Aim->BeginPlay(OwnerCharacter);
}

void ACDoAction_Arrow::DoAction()
{
	if (Aim->IsAvaliable())
		CheckFalse(Aim->IsZoom());

	CheckFalse(State->IsIdleMode()); 
	State->SetActionMode(); 

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRatio, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop(); 
}

void ACDoAction_Arrow::Begin_DoAction()
{
	ICWeapon_Interface* Bow = Cast<ICWeapon_Interface>(OwnerCharacter);
	CheckNull(Bow);

	FVector start, end, direction; 
	Bow->GetLocationAndDirection(start, end, direction); 

	FVector Location = OwnerCharacter->GetMesh()->GetSocketLocation("Arrow");

	FCollisionQueryParams params;
	params.AddIgnoredActor(OwnerCharacter);

	FHitResult hitResult;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_WorldDynamic, params))
	{
		AStaticMeshActor* staticMeshActor = Cast<AStaticMeshActor>(hitResult.GetActor());
		if (!!staticMeshActor)
		{
			UStaticMeshComponent* meshComponent = Cast<UStaticMeshComponent>(staticMeshActor->GetRootComponent());
			{
				if (meshComponent->BodyInstance.bSimulatePhysics)
				{
					direction = staticMeshActor->GetActorLocation() - OwnerCharacter->GetActorLocation();
					direction.Normalize();

					//meshComponent->AddImpulseAtLocation(direction * meshComponent->GetMass() * 100, OwnerCharacter->GetActorLocation());

					return;
				}
			}
		}
	}

	FActorSpawnParameters SpawnParameters;

	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (!!Datas[0].ArrowClass)
	{
		arrowObjectAttach->Destroy();
		arrowObject = GetWorld()->SpawnActor<ACArrow>(Datas[0].ArrowClass, Location, direction.Rotation(), SpawnParameters);
		arrowObject->OffSpecialCollision();
		arrowObject->OnArrowBeginOverlap.AddDynamic(this, &ACDoAction_Arrow::OnArrowBeginOverlap);
		arrowObject->Launch();
	}
}

void ACDoAction_Arrow::End_DoAction()
{
	State->SetIdleMode(); 
	Status->SetMove(); 
}

void ACDoAction_Arrow::HeavyAction()
{
	//CLog::Print("HeavyAttack");

	bHeavyAttack = true;

	arrowObjectAttach->HeavyAction();

	//UParticleSystem* Effect;
	//CHelpers::GetAssetDynamic(&Effect, "ParticleSystem'/Game/Effects/P_prizum.P_prizum'");
	//FTransform transfom;
	//transfom.AddToTranslation(arrowObjectAttach->GetActorLocation());
	//transfom.SetScale3D(FVector(0.3f, 0.3f, 0.3f));
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, transfom);
}

void ACDoAction_Arrow::OnAim()
{
	OwnerCharacter->PlayAnimMontage(Datas[1].AnimMontage, Datas[1].PlayRatio, Datas[1].StartSection);
	Aim->OnZoom(); 
}

void ACDoAction_Arrow::OffAim()
{
	Aim->OffZoom(); 
}

void ACDoAction_Arrow::Spawn()
{
	ICWeapon_Interface* Bow = Cast<ICWeapon_Interface>(OwnerCharacter);
	CheckNull(Bow);

	FVector start, end, direction;
	Bow->GetLocationAndDirection(start, end, direction);

	FVector Location = OwnerCharacter->GetMesh()->GetSocketLocation("Arrow");

	FActorSpawnParameters SpawnParameters;

	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (!!Datas[0].ArrowClass)
	{
		arrowObjectAttach = OwnerCharacter->GetWorld()->SpawnActor<ACArrow>(Datas[0].ArrowClass, Location, direction.Rotation(), SpawnParameters);
		arrowObjectAttach->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true), "Arrow");
	}
}

void ACDoAction_Arrow::Tick(float DelatTime)
{
	Super::Tick(DelatTime);

	Aim->Tick(DelatTime);

	//ICWeapon_Interface* Bow = Cast<ICWeapon_Interface>(OwnerCharacter); 
	//CheckNull(Bow);

	//FVector start, end, direction; 
	//Bow->GetLocationAndDirection(start, end, direction); 
}

void ACDoAction_Arrow::OnArrowBeginOverlap(FHitResult InHItResult)
{
	if (bHeavyAttack == true)
	{
		FDamageEvent e;
		InHItResult.GetActor()->TakeDamage(Datas[0].Power+5, e, OwnerCharacter->GetController(), this);
		bHeavyAttack = false;
	}
	else
	{
		FDamageEvent e;
		InHItResult.GetActor()->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);
	}

	UParticleSystem* hitEffect = Datas[0].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[0].EffectTransform; 
		transform.AddToTranslation(InHItResult.GetActor()->GetActorLocation());

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform); 
	}
}
