#include "AI/CBTDecorator_IsInDashRange.h"
#include "Global.h"
#include "Character/CAIController_Enemy.h"
#include "Character/CPlayer.h"
#include "Character/CEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBTDecorator_IsInDashRange::UCBTDecorator_IsInDashRange()
{
	NodeName = TEXT("CanDash"); 
}

bool UCBTDecorator_IsInDashRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory); 

	auto ControllingPawn = Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
		return false; 

	auto Target = Cast<ACPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ACAIController_Enemy::TargetKey)); 
	if (nullptr == Target)
		return false;
		
	bResult = (Target->GetDistanceTo(ControllingPawn) <= 600);

	return bResult;
}
