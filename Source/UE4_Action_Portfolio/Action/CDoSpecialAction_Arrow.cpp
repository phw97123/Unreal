#include "CDoSpecialAction_Arrow.h"
#include "Global.h"
#include "CArrow.h"	
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "Component/CStatusComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraShakeBase.h"
#include "CWeapon_Interface.h"
#include "Engine/StaticMeshActor.h"

void ACDoSpecialAction_Arrow::DoSpecialAction()
{
	//CLog::Print("SpecialAttack");

	Super::DoSpecialAction();

	CheckFalse(State->IsIdleMode()); 
	State->SetSpecialActionMode(); 

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRatio, Datas[0].StartSection); 
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop(); 

	/*const FDoActionData& data = Datas[0]; 
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection); 

	data.bCanMove ? Status->SetMove() : Status->SetStop(); */
}

void ACDoSpecialAction_Arrow::Begin_DoSpecialAction()
{
	ICWeapon_Interface* Bow = Cast<ICWeapon_Interface>(OwnerCharacter);
	CheckNull(Bow);

	FVector start, end, direction;
	Bow->GetLocationAndDirection(start, end, direction);

	FVector Location = OwnerCharacter->GetMesh()->GetSocketLocation("Arrow");

	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
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
		arrowObject->OnSpecialCollision(); 
		arrowObject->OnEffectBeginOverlap.AddDynamic(this, &ACDoSpecialAction_Arrow::OnEffectBeginOverlap);
		arrowObject->SpecialLaunch();
	}
}

void ACDoSpecialAction_Arrow::End_DoSpecialAction()
{
	Super::End_DoSpecialAction();

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoSpecialAction_Arrow::Spawn()
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

void ACDoSpecialAction_Arrow::RestoreDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f); 
}

void ACDoSpecialAction_Arrow::OnEffectBeginOverlap(FHitResult InHItResult)
{
	FDamageEvent e;
	InHItResult.GetActor()->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);
}