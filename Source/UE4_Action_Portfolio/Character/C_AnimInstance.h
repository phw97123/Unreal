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
	//재생시작이 호출될 때 실행
	virtual void NativeBeginPlay() override; 
	//기본 업데이트 재정의
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


private:
	UFUNCTION()
		void OnActionTypeChanged(EActionType InPrevType, EActionType InNewType);

};
