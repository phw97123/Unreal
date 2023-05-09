#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Health.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API UCUserWidget_Health : public UUserWidget
{
	GENERATED_BODY()
	
public: 
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHealth(float Health, float MaxHealth); 

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateMana(float Mana, float MaxMana);
};
