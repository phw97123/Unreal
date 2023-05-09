#pragma once

#include "CoreMinimal.h"
#include "Action/CDoAction.h"
#include "CDoAction_Throw.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACDoAction_Throw : public ACDoAction
{
	GENERATED_BODY()

protected: 
	virtual void BeginPlay() override;

public: 
	virtual void DoAction() override; 
	virtual void Begin_DoAction() override; 
	virtual void End_DoAction() override; 

	virtual void Tick(float DeltaTime) override; 

private: 
	UFUNCTION()
		void OnThrowBeginOverlap(FHitResult InHitResult); 

	class ACPlayer* player; 
};
