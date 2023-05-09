#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle, Dash, Equip, Action,SpecialAction, Hitted, Dead, Max,
};

// ���̳��� ��Ƽĳ��Ʈ : �������Ʈ���� ��� �����ϰ� �������� �Լ��� ���ε� ������ ��������Ʈ 
// �������� �Լ����� 2���� ���ڸ� �޾� ȣ�� ���ִ� ��� 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_ACTION_PORTFOLIO_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE	bool IsDashMode() { return Type == EStateType::Dash; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsSpecialActionMode() { return Type == EStateType::SpecialAction; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }


public: 
	void SetIdleMode();
	void SetDashMode();
	void SetEquipMode(); 
	void SetActionMode();
	void SetHittedMode(); 
	void SetDeadMode(); 
	void SetSpecialActionMode();

private: 
	void ChangeType(EStateType InType);	
public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public: 
	//�������Ʈ���� �Ҵ��� �� �ְ� �ϴ� ����
	UPROPERTY(BlueprintAssignable)
		FStateTypeChanged OnStateTypeChanged;
private:
	EStateType Type;
};
