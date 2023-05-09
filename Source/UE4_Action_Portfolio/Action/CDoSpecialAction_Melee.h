#pragma once

#include "CoreMinimal.h"
#include "Action/CDoSpecialAction.h"
#include "CDoSpecialAction_Melee.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACDoSpecialAction_Melee : public ACDoSpecialAction
{
	GENERATED_BODY()
	
public: 
	virtual void DoSpecialAction() override;
	virtual void Begin_DoSpecialAction() override; 
	virtual void End_DoSpecialAction() override;

public:
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;

	virtual void OnAttachmentCollision();
	virtual void OffAttachmentCollision();
	
private:
	UFUNCTION()
		void RestoreDilation();


private:
	int32 Index;
	TArray<class ACharacter*> HittedCharacters;



};
