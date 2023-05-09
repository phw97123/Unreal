#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_FindPatrolPos.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API UCBTTaskNode_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
		
public: 
	UCBTTaskNode_FindPatrolPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Search")
		float SerchRadius = 1500.0f;
};
