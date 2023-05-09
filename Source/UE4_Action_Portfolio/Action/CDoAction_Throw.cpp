#include "Action/CDoAction_Throw.h"
#include "Global.h"
#include "CThrow.h"
#include "GameFramework/Character.h"
#include "Component/CStatecomponent.h"
#include "Component/CStatusComponent.h"
#include "Character/CPlayer.h"
#include "GameFramework/ProjectileMovementComponent.h"


void ACDoAction_Throw::BeginPlay()
{
	Super::BeginPlay(); 

	player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)); 
}

void ACDoAction_Throw::DoAction()
{
	CheckFalse(State->IsIdleMode()); 
	State->SetActionMode(); 

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRatio, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop(); 
}

void ACDoAction_Throw::Begin_DoAction()
{
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Throw_Projectile"); 
	FRotator rotator = OwnerCharacter->GetController()->GetControlRotation(); 
	
	FTransform transform = Datas[0].EffectTransform; 
	transform.AddToTranslation(location); 
	transform.SetRotation(FQuat(rotator)); 

	FActorSpawnParameters params; 
	params.Owner = this; 

	ACThrow* throwObject = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACThrow>(Datas[0].ThrowClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Throw::OnThrowBeginOverlap);
	UGameplayStatics::FinishSpawningActor(throwObject, transform);

	FVector Direction = (player->GetActorLocation() - location).GetSafeNormal(); 
	throwObject->Projectile->Velocity = Direction * 1500.0f; 

	//ACThrow* throwObject = GetWorld()->SpawnActor<ACThrow>(ThrowClass, location, rotator, params);
	//throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Throw::OnThrowBeginOverlap);
}

void ACDoAction_Throw::End_DoAction()
{
	State->SetIdleMode(); 
	Status->SetMove(); 
}

void ACDoAction_Throw::Tick(float DeltaTime)
{

}

void ACDoAction_Throw::OnThrowBeginOverlap(FHitResult InHitResult)
{
	FDamageEvent e; 
	InHitResult.GetActor()->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this); 
}
