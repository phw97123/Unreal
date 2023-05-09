#pragma once

#include "CoreMinimal.h"
#include "Character/CEnemy.h"
#include "CEnemy_Boss.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACEnemy_Boss : public ACEnemy
{
	GENERATED_BODY()
		
public:
	ACEnemy_Boss(); 

	void BeginPlay() override; 

	virtual void Tick(float DeltaTime) override;

	TSubclassOf<class UCUserWidget_Health> healthClass;

public: 
	virtual void Attack() override; 
	virtual void EndAttack() override; 
	virtual void TimerCallAttack() override;
	virtual void BossFloorAttack() override; 

	void DanmakuAttack(); 

	virtual void Dash() override; 

public: 
	void PlayDash(); 

private: 
	class UCUserWidget_Health* BossHealthBar; 
	class APawn* playerPawn; 

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACThrow> FireballClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACBossFloorPattern> FloorClass;

private:
	UFUNCTION()
		void OnThrowBeginOverlap(FHitResult InHItResult);
	UFUNCTION()
		void OnFloorBeginOverlap(FHitResult InHItResult);

};
