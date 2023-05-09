#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_TurnToTarget.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API UCBTTaskNode_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()
		
public: 
	UCBTTaskNode_TurnToTarget(); 
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
};
