#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_NumberOfEnemies.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API UCUserWidget_NumberOfEnemies : public UUserWidget
{
	GENERATED_BODY()

public: 
	UFUNCTION(BlueprintCallable)
		int32 CountAliveEnemies(); 

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateNumberOfEnemies(); 

	UFUNCTION(BlueprintCallable)
		void SpawnPotal(); 

public:
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		TSubclassOf<class ACBossStagePotal> BossStagePotalClass;
};
