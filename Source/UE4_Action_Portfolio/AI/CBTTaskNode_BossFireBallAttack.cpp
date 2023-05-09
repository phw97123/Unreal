#include "AI/CBTTaskNode_BossFireBallAttack.h"
#include "Global.h"
#include "Character/CAIController_Enemy_Boss.h"
#include "Character/CEnemy.h"

UCBTTaskNode_BossFireBallAttack::UCBTTaskNode_BossFireBallAttack()
{
	bNotifyTick = true;
	IsAttacking = false;

	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UCBTTaskNode_BossFireBallAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Enemy = Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn()); 
	if (nullptr == Enemy)
		return EBTNodeResult::Failed; 

	Enemy->TimerCallAttack();
	IsAttacking = true;

	Enemy->OnAttackEnd.AddLambda([this]()->void
		{
			IsAttacking = false;
		});

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_BossFireBallAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecondes)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSecondes);

	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

