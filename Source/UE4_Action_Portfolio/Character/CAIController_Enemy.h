#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CAIController_Enemy.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACAIController_Enemy : public AAIController
{
	GENERATED_BODY()
	
public: 
	ACAIController_Enemy(); 
	
	void BeginPlay() override; 

	virtual void OnPossess(APawn* InPawn) override;

	class UAISenseConfig_Sight* SightConfig; 

	static const FName HomePosKey; 
	static const FName PatrolPosKey; 
	static const FName TargetKey; 
	static const FName CanSeePlayer; 
	static const FName TargetLocation; 

private: 
	UPROPERTY()
		class UBehaviorTree* BTAsset; 

	UPROPERTY()
		class UBlackboardData* BBAsset;

	UPROPERTY()
		class UBehaviorTreeComponent* BTComp;

	//UPROPERTY()
	//	class UBlackboardComponent* blackboard;

public:
	class UBlackboardComponent* GetBlackboard() const;

	UFUNCTION()
		void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);

	UFUNCTION()
		void SetPerceptionSystem(); 

	UFUNCTION()
		void StopBehaviorTree(); 


// AI Perception º¯¼ö
public: 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AISightRadius = 1000.0f; 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AILoseSightRadius = 50.0f; 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AIFieldOfView = 90.0f; 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AISightAge = 5.0f; 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float AILastSeenLocation = 900.0f; 

private: 
	class ACEnemy* enemy;
};
