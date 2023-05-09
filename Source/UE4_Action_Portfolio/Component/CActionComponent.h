#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"

UENUM(BlueprintType)
enum class EActionType: uint8
{
	Unarmed, Sword, Bow, Fist, Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_ACTION_PORTFOLIO_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

private: 
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		class UCActionData* Datas[(int32)EActionType::Max];

public: 
	UFUNCTION(BlueprintPure)
		FORCEINLINE	class UCActionData* GetCurrent() { return Datas[(int32)Type]; }

public: 
	UFUNCTION(BlueprintPure)
		FORCEINLINE	 bool IsUnarmedMode() { return Type == EActionType::Unarmed; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE	 bool IsSwordMode() { return Type == EActionType::Sword; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE	 bool IsBowMode() { return Type == EActionType::Bow; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE	 bool IsFistMode() { return Type == EActionType::Fist; }

public:	
	UCActionComponent();

	void SetUnarmedMode(); 
	void SetSwordMode(); 
	void SetBowMode(); 
	void SetFistMode(); 

	EActionType GetType() { return Type; }

	void OffAllCollision(); 

	void SetMode(EActionType InType);
	void ChangeType(EActionType InNewType);

public: 
	void DoAction(); 
	void DoSpecialAction(); 

	void DoAim(); 
	void UnDoAim();

private:
	void SetAimMode(bool InAim); 


protected:
	virtual void BeginPlay() override;

public: 
	FActionTypeChanged OnActionTypeChanged;
		
private:
	EActionType Type;

public: 
	class ACPlayer* player; 
};
