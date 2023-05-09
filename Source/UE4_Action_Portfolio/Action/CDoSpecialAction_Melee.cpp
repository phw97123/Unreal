#include "CDoSpecialAction_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "Component/CStatusComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraShakeBase.h"

void ACDoSpecialAction_Melee::DoSpecialAction()
{
	//CLog::Print("SpecialAttack");

	Super::DoSpecialAction(); 
	CheckFalse(Datas.Num() > 0);

	CheckFalse(State->IsIdleMode()); 
	State->SetSpecialActionMode(); 

	const FDoActionData& data = Datas[0]; 
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	data.bCanMove ? Status->SetMove() : Status->SetStop(); 
}

void ACDoSpecialAction_Melee::Begin_DoSpecialAction()
{
	Super::Begin_DoSpecialAction(); 

	const FDoActionData& data = Datas[0];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	data.bCanMove ? Status->SetMove() : Status->SetStop();

}

void ACDoSpecialAction_Melee::End_DoSpecialAction()
{
	Super::End_DoSpecialAction(); 

	const FDoActionData& data = Datas[0];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoSpecialAction_Melee::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{

	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);
	CheckNull(InOtherCharacter);

	for (const ACharacter* other : HittedCharacters)
	{
		if (InOtherCharacter == other)
			return;
	}

	HittedCharacters.Add(InOtherCharacter);

	UParticleSystem* hitEffect = Datas[0].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[0].EffectTransform;
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	float hitStop = Datas[0].HitStop;
	if (FMath::IsNearlyZero(hitStop) == false)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-3f);
		UKismetSystemLibrary::K2_SetTimer(this, "RestorDilation", hitStop * 1e-3f, false);
	}

	TSubclassOf<UCameraShakeBase> shake = Datas[0].ShakeClass;
	if (shake != NULL)
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(shake);

	FDamageEvent e;
	InOtherCharacter->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);
}

void ACDoSpecialAction_Melee::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
}

void ACDoSpecialAction_Melee::OnAttachmentCollision()
{
}

void ACDoSpecialAction_Melee::OffAttachmentCollision()
{
	HittedCharacters.Empty(); 
}

void ACDoSpecialAction_Melee::RestoreDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f); 
}
