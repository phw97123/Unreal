#pragma once

#include "CoreMinimal.h"
#include "Character/CEnemy.h"
#include "CEnemy_Wizard.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACEnemy_Wizard : public ACEnemy
{
	GENERATED_BODY()

public: 
	ACEnemy_Wizard(); 

	void BeginPlay() override; 

	virtual void Attack() override; 
	virtual void EndAttack() override; 
	
};