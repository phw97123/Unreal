#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CEnemySpawner.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACEnemySpawner : public AActor
{
	GENERATED_BODY()

private: 
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* SpawnEffect;
	
public:	
	ACEnemySpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public: 
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ACEnemy> EnemyClass;
	
	UFUNCTION()
		void EffectActivate(); 

public:
	UFUNCTION()
		void SpawnEnemyAtChildActor(); 

};
