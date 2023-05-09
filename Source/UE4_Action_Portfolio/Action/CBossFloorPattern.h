#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBossFloorPattern.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFloorBeginOverlap, FHitResult, InHitResult);


UCLASS()
class UE4_ACTION_PORTFOLIO_API ACBossFloorPattern : public AActor
{
	GENERATED_BODY()
	
private: 
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene; 

	UPROPERTY(VisibleDefaultsOnly)
		class UDecalComponent* Decal; 

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Effect;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

public:	
	ACBossFloorPattern();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void StartDelay(float DelayTime); 

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(BlueprintAssignable)
		FFloorBeginOverlap OnFloorBeginOverlap;

private:
	FTimerHandle DelayTimerHandle;

};
