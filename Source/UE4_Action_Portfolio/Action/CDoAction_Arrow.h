#pragma once

#include "CoreMinimal.h"
#include "Action/CDoAction.h"
#include "CDoAction_Arrow.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACDoAction_Arrow : public ACDoAction
{
	GENERATED_BODY()
protected: 
	virtual void BeginPlay() override; 

public: 
	virtual void DoAction() override; 
	virtual	void Begin_DoAction() override; 
	virtual	void End_DoAction() override; 

	virtual void HeavyAction() override;

	virtual void OnAim() override; 
	virtual void OffAim() override; 

	virtual void Spawn() override;

	virtual void Tick(float DeltaTime) override;

private: 
	UFUNCTION()
		void OnArrowBeginOverlap(FHitResult InHItResult); 

private: 
	class UCAim* Aim; 
	class ACArrow* arrowObjectAttach;
	class ACArrow* arrowObject;

	bool bHeavyAttack; 
};
