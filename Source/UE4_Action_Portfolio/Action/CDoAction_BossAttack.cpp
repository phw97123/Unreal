#include "Action/CDoAction_BossAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "Component/CStatusComponent.h"
#include "Character/CAIController_Enemy_Boss.h"
#include "AI/CBTTaskNode_RandomBossAttack.h"
#include "BehaviorTree/BlackboardComponent.h"

void ACDoAction_BossAttack::DoAction()
{
	Super::DoAction();

	State->SetActionMode(); 

	BossAiController = Cast<ACAIController_Enemy_Boss>(OwnerCharacter->GetController()); 

	index = BossAiController->GetBlackboard()->GetValueAsInt(BossAiController->BossAttack); 

	const FDoActionData& data = Datas[index]; 
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection); 

	data.bCanMove ? Status->SetMove() : Status->SetStop(); 
}

void ACDoAction_BossAttack::End_DoAction()
{
	Super::End_DoAction(); 

	State->SetIdleMode(); 
	Status->SetMove(); 
}

void ACDoAction_BossAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
}

void ACDoAction_BossAttack::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);
	CheckNull(InOtherCharacter);

	for (const ACharacter* other : HittedCharacters)
	{
		if (InOtherCharacter == other)
			return;
	}

	HittedCharacters.Add(InOtherCharacter);

	UParticleSystem* hitEffect = Datas[index].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[index].EffectTransform;
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	TSubclassOf<UCameraShakeBase> shake = Datas[index].ShakeClass;
	if (shake != NULL)
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(shake);

	FDamageEvent e;
	InOtherCharacter->TakeDamage(Datas[index].Power, e, OwnerCharacter->GetController(), this);
}

void ACDoAction_BossAttack::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
}

void ACDoAction_BossAttack::OnAttachmentCollision()
{

}

void ACDoAction_BossAttack::OffAttachmentCollision()
{
	HittedCharacters.Empty();
}


