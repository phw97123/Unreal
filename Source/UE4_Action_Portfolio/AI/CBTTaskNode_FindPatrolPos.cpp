#include "AI/CBTTaskNode_FindPatrolPos.h"
#include "Global.h"
#include "Character/CAIController_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "CBTTaskNode_FindPatrolPos.h"

UCBTTaskNode_FindPatrolPos::UCBTTaskNode_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UCBTTaskNode_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto Controller = Cast<ACAIController_Enemy>(OwnerComp.GetAIOwner()); 
	auto enemy = Controller->GetPawn(); 

	if (nullptr == enemy)
	{
		CLog::Print("Enemy Initialize Failed"); 
		return EBTNodeResult::Failed; 
	}
	
	UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (nullptr == NavSystem)
	{
		CLog::Print("No Enemy in Navi"); 
		return EBTNodeResult::Failed; 
	}

	FVector const Origin = enemy->GetActorLocation();	
	FNavLocation NextPatrol; 

	if (NavSystem->GetRandomReachablePointInRadius(Origin, SerchRadius, NextPatrol, nullptr))
	{
		Controller->GetBlackboard()->SetValueAsVector(ACAIController_Enemy::TargetLocation, NextPatrol.Location); 
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); 
	return EBTNodeResult::Succeeded; 
}
