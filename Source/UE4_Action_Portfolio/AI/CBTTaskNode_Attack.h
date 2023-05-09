#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Attack.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API UCBTTaskNode_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public: 
	UCBTTaskNode_Attack(); 

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected: 
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSecondes) override; 

private: 
	bool IsAttacking = false; 

};
