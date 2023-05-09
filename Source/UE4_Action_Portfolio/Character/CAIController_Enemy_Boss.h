#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CAIController_Enemy_Boss.generated.h"

UENUM(BlueprintType)
enum class EBossState : uint8
{
	Attack, Invincibility,
};

UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	PhaseOne, PhaseTwo, PhaseThree,
};


UCLASS()
class UE4_ACTION_PORTFOLIO_API ACAIController_Enemy_Boss : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		class UBehaviorTree* BTAsset;

	UPROPERTY()
		class UBlackboardData* BBAsset;

	UPROPERTY()
		class UBehaviorTreeComponent* BehaviorTreeComp;

	UFUNCTION()
	virtual void OnPossess(APawn* InPawn) override;

public:
	ACAIController_Enemy_Boss(); 

	void BeginPlay() override; 

public: 
	UFUNCTION()
		void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);

	UFUNCTION()
		void SetPerceptionSystem();

	UFUNCTION()
		void SetBossState(EBossState state); 

	UFUNCTION()
		void SetBossPhase(EBossPhase phase); 


	class UBlackboardComponent* GetBlackboard() const; 

	UFUNCTION()
		class UBehaviorTreeComponent* GetBehaviorComp() const;

	class UAISenseConfig_Sight* SightConfig;

	static const FName HomePosKey;
	static const FName TargetKey;
	static const FName CanSeePlayer;
	static const FName TargetLocation;
	static const FName EBossStateType; 
	static const FName EBossPhaseType;
	static const FName BossAttack; 


public: 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AISightRadius = 2000.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AILoseSightRadius = 50.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AIFieldOfView = 90.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AISightAge = 5.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AILastSeenLocation = 900.0f;
private: 
	class ACEnemy_Boss* Enemy_Boss; 
};
