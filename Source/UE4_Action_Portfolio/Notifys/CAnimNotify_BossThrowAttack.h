#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_BossThrowAttack.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API UCAnimNotify_BossThrowAttack : public UAnimNotify
{
	GENERATED_BODY()

public: 
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
