#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_DamageNumber.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API UCUserWidget_DamageNumber : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateDamage(float Damage);
};
