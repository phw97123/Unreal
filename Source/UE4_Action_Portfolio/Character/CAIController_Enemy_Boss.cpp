#include "Character/CAIController_Enemy_Boss.h"
#include "Global.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "CEnemy_Boss.h"
#include "CPlayer.h"

const FName ACAIController_Enemy_Boss::HomePosKey(TEXT("HomePos"));
const FName ACAIController_Enemy_Boss::TargetKey(TEXT("Target"));
const FName ACAIController_Enemy_Boss::CanSeePlayer(TEXT("CanSeePlayer"));
const FName ACAIController_Enemy_Boss::TargetLocation(TEXT("TargetLocation"));
const FName ACAIController_Enemy_Boss::EBossStateType(TEXT("EBossState")); 
const FName ACAIController_Enemy_Boss::EBossPhaseType(TEXT("EBossPhase")); 
const FName ACAIController_Enemy_Boss::BossAttack(TEXT("BossAttack")); 

ACAIController_Enemy_Boss::ACAIController_Enemy_Boss()
{
	SetPerceptionSystem(); 

	CHelpers::CreateActorComponent<UBehaviorTreeComponent>(this, &BehaviorTreeComp, "BehaviorTreeComp"); 
}

void ACAIController_Enemy_Boss::BeginPlay()
{
	Super::BeginPlay();
}

void ACAIController_Enemy_Boss::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn); 

	Enemy_Boss = Cast<ACEnemy_Boss>(InPawn); 

	BTAsset = Enemy_Boss->BTAsset; 
	BBAsset = Enemy_Boss->BBAsset;

	BehaviorTreeComp->RegisterComponent(); 
	BehaviorTreeComp->StartTree(*BTAsset); 

	RunBehaviorTree(BTAsset); 
}

void ACAIController_Enemy_Boss::OnTargetDetected(AActor* actor, FAIStimulus const Stimulus)
{
	if (auto const player = Cast<ACPlayer>(actor))
	{
		//���������� �����ϸ� �����忡 True ���� �־���
		GetBlackboard()->SetValueAsBool(CanSeePlayer, Stimulus.WasSuccessfullySensed());
		GetBlackboard()->SetValueAsObject(TargetKey, actor);
	}
}

void ACAIController_Enemy_Boss::SetPerceptionSystem()
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
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ACAIController_Enemy_Boss::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void ACAIController_Enemy_Boss::SetBossState(EBossState state)
{
	if (state == EBossState::Attack)
	{
		GetBlackboard()->SetValueAsEnum(EBossStateType, 0); 
	}
	else if (state == EBossState::Invincibility)
	{
		GetBlackboard()->SetValueAsEnum(EBossStateType, 1); 
	}
}

void ACAIController_Enemy_Boss::SetBossPhase(EBossPhase phase)
{
	if (phase == EBossPhase::PhaseOne)
	{
		GetBlackboard()->SetValueAsEnum(EBossPhaseType, 0); 
	}
	else if (phase == EBossPhase::PhaseTwo)
	{
		GetBlackboard()->SetValueAsEnum(EBossPhaseType, 1); 
	}
	else if (phase == EBossPhase::PhaseThree)
	{
		GetBlackboard()->SetValueAsEnum(EBossPhaseType, 2); 
	}
}

UBlackboardComponent* ACAIController_Enemy_Boss::GetBlackboard() const
{
	return Blackboard;
}

UBehaviorTreeComponent* ACAIController_Enemy_Boss::GetBehaviorComp() const
{
	return BehaviorTreeComp;
}
