#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/CCharacter_Interface.h"
#include "Component/CStateComponent.h"
#include "CEnemy.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACEnemy : public ACharacter, public ICCharacter_Interface
{
	GENERATED_BODY()

protected: 
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* DamageNumber;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* GroggyEffect;

protected: 
	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* Action; 

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontageComponent* Montages;

public:
	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State; 

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

public: 
	UPROPERTY(EditDefaultsOnly)
		class UBehaviorTree* BTAsset;

	UPROPERTY(EditDefaultsOnly)
		class UBlackboardData* BBAsset; 

private: 
	UPROPERTY(EditAnywhere, Category = "Hitted")
		float LaunchAmount = 300.0f;


public:
	ACEnemy();

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType); 

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private: 
	void Hitted(); 
	void Dead(); 
	virtual void Dash() {}

public:
	virtual void Attack() {}
	virtual void TimerCallAttack() {}
	virtual void BossFloorAttack() {}
	virtual void EndAttack() {}

	FOnAttackEndDelegate OnAttackEnd;

public: 
	virtual void ChangeColor(FLinearColor InColor) override;
	virtual void Begin_Dead() override; 
	virtual void End_Dead() override; 

private:
	void SpawnPotion(); 

private: 
	UFUNCTION()
		void RestoreColor(); 

protected:
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;

private: 
	class AController* DamageInstigator; 
	float DamageValue;
};
