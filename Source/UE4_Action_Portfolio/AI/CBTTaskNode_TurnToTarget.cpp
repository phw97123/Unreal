#include "CBTTaskNode_TurnToTarget.h"
#include "Global.h"
#include "Character/CAIController_Enemy.h"
#include "Character/CEnemy.h"
#include "Character/CPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBTTaskNode_TurnToTarget::UCBTTaskNode_TurnToTarget()
{
	NodeName = TEXT("Turn"); 
}

EBTNodeResult::Type UCBTTaskNode_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto enemy = Cast<ACEnemy>(OwnerComp.GetAIOwner()->GetPawn()); 
	if (nullptr == enemy)
		return EBTNodeResult::Failed; 

	auto Target = Cast<ACPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ACAIController_Enemy::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed; 

	FVector LookVector = Target->GetActorLocation() - enemy->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRoT = FRotationMatrix::MakeFromX(LookVector).Rotator(); 
	enemy->SetActorRotation(FMath::RInterpTo(enemy->GetActorRotation(), TargetRoT, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}
