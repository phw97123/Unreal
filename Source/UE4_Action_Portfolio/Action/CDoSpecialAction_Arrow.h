#pragma once

#include "CoreMinimal.h"
#include "Action/CDoSpecialAction.h"
#include "CDoSpecialAction_Arrow.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACDoSpecialAction_Arrow : public ACDoSpecialAction
{
	GENERATED_BODY()
	
public: 
	virtual void DoSpecialAction() override;
	virtual void Begin_DoSpecialAction() override;
	virtual void End_DoSpecialAction() override;

	virtual void Spawn() override;
private:
	UFUNCTION()
		void RestoreDilation(); 

private: 
	UFUNCTION()
		void OnEffectBeginOverlap(FHitResult InHItResult);

private: 
	TArray<class ACharacter*> HittedCharacters;
	class ACArrow* arrowObjectAttach;
	class ACArrow* arrowObject;
};
