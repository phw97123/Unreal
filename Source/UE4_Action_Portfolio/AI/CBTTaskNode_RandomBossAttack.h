#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_RandomBossAttack.generated.h"


UCLASS()
class UE4_ACTION_PORTFOLIO_API UCBTTaskNode_RandomBossAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public: 
	UCBTTaskNode_RandomBossAttack(); 
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		int32 MinValue;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
		int32 MaxValue;

	UPROPERTY()
		int32 RandomInt;

	class ACAIController_Enemy_Boss* BossAiController;

};
