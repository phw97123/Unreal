#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_BossStateAndPhase.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API UCBTService_BossStateAndPhase : public UBTService
{
	GENERATED_BODY()
	
public: 
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	class ACAIController_Enemy_Boss* BossAIContorller;
};
