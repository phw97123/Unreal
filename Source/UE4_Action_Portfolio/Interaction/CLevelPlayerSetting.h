#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CLevelPlayerSetting.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACLevelPlayerSetting : public AActor
{
	GENERATED_BODY()
private: 
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box; 
public:	
	ACLevelPlayerSetting();

public: 
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override; 

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
//
//private: 
//	UPROPERTY()
//		class ACBossStagePotal* BossPotal; 
};
