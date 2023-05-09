#pragma once

#include "CoreMinimal.h"
#include "Action/CWeapon_Interface.h"
#include "Character/CCharacter_Interface.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "CPlayer.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACPlayer : public ACharacter,public ICWeapon_Interface, public ICCharacter_Interface
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;
public:
	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly)
		class UChildActorComponent* MainCamChild;

	//Sword Skill 
	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* SubCamera1;

	UPROPERTY(VisibleDefaultsOnly)
		class UChildActorComponent* SubCamChild1;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* SubCamera2;

	UPROPERTY(VisibleDefaultsOnly)
		class UChildActorComponent* SubCamChild2;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* SubCamera3;

	UPROPERTY(VisibleDefaultsOnly)
		class UChildActorComponent* SubCamChild3;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* SubCamera4;

	UPROPERTY(VisibleDefaultsOnly)
		class UChildActorComponent* SubCamChild4;

	//Arrow Skill 
	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* SubCamera5;

	UPROPERTY(VisibleDefaultsOnly)
		class UChildActorComponent* SubCamChild5;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontageComponent* Montage;

	UPROPERTY(VisibleDefaultsOnly)
		class UUCTargetComponent* Target;

	UPROPERTY(VisibleDefaultsOnly)
		class UCFeetComponent* Feet;

public:
	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* Action;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCUserWidget_CrossHair> CrossHairClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCUserWidget_Health> ActionBarClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCUserWidget_SkillSlot> SkillSlotClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCUserWidget_NumberOfEnemies> numberofEnemiesClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCUserWidget_SkillSlot> DashSkillSlotClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCUserWidget_ActionList> ActionListClass; 

public: 
	FORCEINLINE class UCUserWidget_ActionList* GetActionList() { return ActionList; }

public: 
	FORCEINLINE class ACAttachment* GetAttachment() { return attachment; }

public:
	ACPlayer();

	virtual void GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//입력, 축 바인딩 함수
private: 
	void OnMoveForward(float InAxis); 
	void OnMoveRight(float InAxis); 
	void OnHorizontalLook(float InAxis); 
	void OnVerticalLook(float InAxis); 
	void OnZoom(float InAxis);
	void OnDash(); 
	void OnTarget(); 
	
	void OnDoAction(); 
	void OnSpecialAction();

	void OnAim();
	void OffAim();

	void OnViewActionList(); 
	void OffViewActionList(); 

private: 
	UFUNCTION()
		void OnStateTypeChanged(EStateType InprevType, EStateType InNewType);

private:
	UFUNCTION()
	void Begin_Dash();

	void Hitted(); 
	void Dead(); 

	UFUNCTION()
		bool CanDash() const;
public:
	UFUNCTION()
	void End_Dash();

	virtual	void ChangeColor(FLinearColor InColor) override;
	virtual void Begin_Dead() override;
	virtual void End_Dead() override;

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private: 
	UPROPERTY(EditDefaultsOnly, Category = "Dash")
		float DashDuration = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
		float DashSpeed = 1000.0f; 

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
		bool bIsDashing = false;
private:
	class ACAttachment* attachment; 
	class ACPickUp* Item;

public: 
	void SetPickUp(bool val);
	bool GetPickUp() { return bPickUp; }

	void SetbItemOverlap(bool val); 
	bool GetItemOverlap() { return bItemOverlap; }

	void SetWeaponType(int Type) { WeaponType = Type;  }
	int GetWeaponType() { return WeaponType; }

private: 
	UFUNCTION()
		void OnSword();

	UFUNCTION()
		void OnBow(); 


public: 
	UFUNCTION()
		void PickUpHealthPotion(float InAmount);

	UFUNCTION()
		void PickUpManaPotion(float InAmount); 

	UFUNCTION()
		void OnSpringArmRotation(); 

	UFUNCTION()
		void OffSpringArmRotation(); 
	
	UFUNCTION()
		void SpringArmChange(FRotator rotator, float TargetArmLength);

	UFUNCTION()
		void VisibillityNumberOfEnemies(); 

private: 
	bool bPickUp; 
	bool bItemOverlap;
	int WeaponType;
	bool bInteraction; 

private: 
	class AController* DamageInstigator; 
	float DamageValue; 

private:
	class UCUserWidget_CrossHair* CrossHair;
	class UCUserWidget_Health* ActionBar; 
	class UCUserWidget_NumberOfEnemies* numberofEnemies;
	class UCUserWidget_SkillSlot* DashSkillSlot;
	class UCUserWidget_ActionList* ActionList; 

public: 
	class UCUserWidget_SkillSlot* SkillSlot; 

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float SkillCoolDown = 8.0f; 

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float DashSkillCoolDown = 5.0f; 

	void EquipSkillSlot(); 
};
