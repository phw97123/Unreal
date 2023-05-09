#include "AI/CBTTaskNode_Attack.h"
#include "Global.h"
#include "Character/CAIController_Enemy.h"
#include "Character/CEnemy.h"

UCBTTaskNode_Attack::UCBTTaskNode_Attack()
{
	bNotifyTick = true; 
	IsAttacking = false;

	NodeName = TEXT("Attack"); 
}

EBTNodeResult::Type UCBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory); 

	auto enemy = Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn()); 
	if (nullptr == enemy)
		return EBTNodeResult::Failed; 
	
	enemy->Attack();
	IsAttacking = true;
	enemy->OnAttackEnd.AddLambda([this]()->void {
		IsAttacking = false;
		});
	
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecondes)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSecondes); 

	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
