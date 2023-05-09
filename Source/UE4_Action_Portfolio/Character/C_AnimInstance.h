#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Component/CActionComponent.h"
#include "Component/CFeetComponent.h"
#include "C_AnimInstance.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API UC_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected: 
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EActionType ActionType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FFeetData FeetData; 

public: 
	//��������� ȣ��� �� ����
	virtual void NativeBeginPlay() override; 
	//�⺻ ������Ʈ ������
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


private:
	UFUNCTION()
		void OnActionTypeChanged(EActionType InPrevType, EActionType InNewType);

};
