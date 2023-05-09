#include "Character/CAIController_Enemy.h"
#include "Global.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "CEnemy.h"
#include "CPlayer.h"

const FName ACAIController_Enemy::HomePosKey(TEXT("HomePos")); 
const FName ACAIController_Enemy::PatrolPosKey(TEXT("PatrolPos"));
const FName ACAIController_Enemy::TargetKey(TEXT("Target"));
const FName ACAIController_Enemy::CanSeePlayer(TEXT("CanSeePlayer"));
const FName ACAIController_Enemy::TargetLocation(TEXT("TargetLocation")); 

ACAIController_Enemy::ACAIController_Enemy()
{
	SetPerceptionSystem(); 

	CHelpers::CreateActorComponent<UBehaviorTreeComponent>(this, &BTComp, "BTComp");
	//CHelpers::CreateActorComponent<UBlackboardComponent>(this, &blackboard, "blackboard");
}

void ACAIController_Enemy::BeginPlay()
{
	Super::BeginPlay(); 
}

void ACAIController_Enemy::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn); 

	enemy = Cast<ACEnemy>(InPawn);

	BTAsset = enemy->BTAsset;
	BBAsset = enemy->BBAsset;

	//RunBehaviorTree(BTAsset);
	
	BTComp->StartTree(*BTAsset);

	BTComp->RegisterComponent();

	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation()); 
	}
}

UBlackboardComponent* ACAIController_Enemy::GetBlackboard() const
{
	return Blackboard;
}


void ACAIController_Enemy::OnTargetDetected(AActor* actor, FAIStimulus const Stimulus)
{
	if (auto const player = Cast<ACPlayer>(actor))
	{
		//성공적으로 감지하면 블랙보드에 True 값을 넣어줌
		GetBlackboard()->SetValueAsBool(CanSeePlayer, Stimulus.WasSuccessfullySensed()); 
		GetBlackboard()->SetValueAsObject(TargetKey,actor);
	}
}

void ACAIController_Enemy::SetPerceptionSystem()
{
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));

	//시야 반경
	SightConfig->SightRadius = AISightRadius;
	//시각 상실 반경
	SightConfig->LoseSightRadius = SightConfig->SightRadius + AILoseSightRadius; 
	//주변 시야각
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView; 
	//자극이 잊히기까지의 시간 (0이면 잊지 않음)
	SightConfig->SetMaxAge(AISightAge); 
	//InvalidRange(기본값)가 아닌 경우 마지막으로 본 위치의 이 범위 내에 있으면 이미 본 대상을 항상 볼 수 있습니다.
	SightConfig->AutoSuccessRangeFromLastSeenLocation = AILastSeenLocation; 

	//소속별 탐지 적
	SightConfig->DetectionByAffiliation.bDetectEnemies = true; 
	//소속별 탐지 중립
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	//소속별 탐지 팀
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true; 

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation()); 
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ACAIController_Enemy::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig); 
}

void ACAIController_Enemy::StopBehaviorTree()
{
	if (BTComp && BTComp->IsRunning())
	{
		BTComp->StopTree(EBTStopMode::Forced);
	}
}

