#include "AI/CBTService_BossStateAndPhase.h"
#include "Global.h"
#include "Character/CAIController_Enemy_Boss.h"
#include "Character/CEnemy_Boss.h"
#include "Component/CStatusComponent.h"

void UCBTService_BossStateAndPhase::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds); 

	BossAIContorller = Cast<ACAIController_Enemy_Boss>(OwnerComp.GetAIOwner());
	if (BossAIContorller == nullptr)
	{
		return;	
	}

	ACEnemy_Boss* Boss = Cast<ACEnemy_Boss>(BossAIContorller->GetPawn());
	if (Boss == nullptr)
	{
		return;
	}

	float bossHealth = Boss->Status->GetHealth();

	if (bossHealth > 300 && bossHealth <= 400)
	{
		BossAIContorller->SetBossState(EBossState::Attack);
		BossAIContorller->SetBossPhase(EBossPhase::PhaseOne);
	}
	else if (bossHealth > 250 && bossHealth <= 300)
	{
		BossAIContorller->SetBossState(EBossState::Invincibility);
	}
	else if (bossHealth > 150 && bossHealth <= 250)
	{
		BossAIContorller->SetBossState(EBossState::Attack);
		BossAIContorller->SetBossPhase(EBossPhase::PhaseTwo);
	}
	else if (bossHealth > 100 && bossHealth <= 150)
	{
		BossAIContorller->SetBossState(EBossState::Invincibility);
	}
	else if (bossHealth <= 100)
	{
		BossAIContorller->SetBossState(EBossState::Attack);
		BossAIContorller->SetBossPhase(EBossPhase::PhaseThree);
	}
}
