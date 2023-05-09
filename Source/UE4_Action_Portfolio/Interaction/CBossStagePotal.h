#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBossStagePotal.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACBossStagePotal : public AActor
{
	GENERATED_BODY()
private: 
	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* PotalEffect;

	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* TriggerVolume;
	
public:	
	ACBossStagePotal();

protected:
	virtual void BeginPlay() override;

public: 
	UFUNCTION()
		virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION()
		void SpawnBossMonster(); 

	UFUNCTION()
		void ReMoveWidget(); 

private: 
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACEnemy_Boss> BossClass; 

	class UCUserWidget_LoadingScreen* Loading;
};
