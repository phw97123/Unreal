#include "AI/CBTTaskNode_RandomBossAttack.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CAIController_Enemy_Boss.h"

UCBTTaskNode_RandomBossAttack::UCBTTaskNode_RandomBossAttack()
{
	NodeName = "RandomBossAttack";
}


EBTNodeResult::Type UCBTTaskNode_RandomBossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent(); 
	if (BlackboardComp == nullptr)
		return EBTNodeResult::Failed;
	
	BossAiController = Cast<ACAIController_Enemy_Boss>(OwnerComp.GetAIOwner()); 
	if (BossAiController == nullptr)
		return EBTNodeResult::Failed;

	RandomInt = FMath::RandRange(MinValue, MaxValue); 

	BlackboardComp->SetValueAsInt(BossAiController->BossAttack, RandomInt); 

	//CLog::Print(BlackboardComp->GetValueAsInt(BossAiController->BossAttack));

	return EBTNodeResult::Succeeded; 
}
