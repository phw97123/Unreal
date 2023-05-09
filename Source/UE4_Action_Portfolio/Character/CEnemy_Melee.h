#pragma once

#include "CoreMinimal.h"
#include "Character/CEnemy.h"
#include "CEnemy_Melee.generated.h"


UCLASS()
class UE4_ACTION_PORTFOLIO_API ACEnemy_Melee : public ACEnemy
{
	GENERATED_BODY()

public: 
	ACEnemy_Melee();

	void BeginPlay() override;

	virtual void Attack() override;
	virtual void EndAttack() override;

};
