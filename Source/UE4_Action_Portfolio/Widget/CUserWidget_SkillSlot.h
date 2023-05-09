#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_SkillSlot.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API UCUserWidget_SkillSlot : public UUserWidget
{
	GENERATED_BODY()
public: 
	UFUNCTION(BlueprintImplementableEvent)
	void SwordSkillSlot(); 

	UFUNCTION(BlueprintImplementableEvent)
	void BowSkillSlot(); 

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateDashSkillSlot(); 
};
