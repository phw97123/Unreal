#pragma once

#include "CoreMinimal.h"
#include "Action/CDoAction.h"
#include "CDoAction_BossAttack.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACDoAction_BossAttack : public ACDoAction
{
	GENERATED_BODY()
	
public: 
	virtual void DoAction() override; 
	virtual void End_DoAction() override; 

	virtual void Tick(float DeltaTime) override;

public:
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser,
		class ACharacter* InOtherCharacter) override;
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser,
		class ACharacter* InOtherCharacter) override;

	virtual void OnAttachmentCollision();
	virtual void OffAttachmentCollision();
private: 
	TArray<class ACharacter*> HittedCharacters;
	class ACAIController_Enemy_Boss* BossAiController;

	int32 index; 

};
