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
		//���������� �����ϸ� �����忡 True ���� �־���
		GetBlackboard()->SetValueAsBool(CanSeePlayer, Stimulus.WasSuccessfullySensed()); 
		GetBlackboard()->SetValueAsObject(TargetKey,actor);
	}
}

void ACAIController_Enemy::SetPerceptionSystem()
{
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));

	//�þ� �ݰ�
	SightConfig->SightRadius = AISightRadius;
	//�ð� ��� �ݰ�
	SightConfig->LoseSightRadius = SightConfig->SightRadius + AILoseSightRadius; 
	//�ֺ� �þ߰�
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView; 
	//�ڱ��� ����������� �ð� (0�̸� ���� ����)
	SightConfig->SetMaxAge(AISightAge); 
	//InvalidRange(�⺻��)�� �ƴ� ��� ���������� �� ��ġ�� �� ���� ���� ������ �̹� �� ����� �׻� �� �� �ֽ��ϴ�.
	SightConfig->AutoSuccessRangeFromLastSeenLocation = AILastSeenLocation; 

	//�ҼӺ� Ž�� ��
	SightConfig->DetectionByAffiliation.bDetectEnemies = true; 
	//�ҼӺ� Ž�� �߸�
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	//�ҼӺ� Ž�� ��
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

