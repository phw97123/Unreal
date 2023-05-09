#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_EnemyEndAction.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API UCAnimNotify_EnemyEndAction : public UAnimNotify
{
	GENERATED_BODY()
	
public: 
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
