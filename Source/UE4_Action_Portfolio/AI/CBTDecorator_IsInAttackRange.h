#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTDecorator_IsInAttackRange.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API UCBTDecorator_IsInAttackRange : public UBTDecorator
{
	GENERATED_BODY()
	
public: 
	UCBTDecorator_IsInAttackRange(); 

protected: 
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)const override;

};
