#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTDecorator_IsInDashRange.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API UCBTDecorator_IsInDashRange : public UBTDecorator
{
	GENERATED_BODY()
	
public: 
	UCBTDecorator_IsInDashRange(); 
protected: 
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override; 
};
