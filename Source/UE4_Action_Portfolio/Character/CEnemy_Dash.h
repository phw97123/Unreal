#pragma once

#include "CoreMinimal.h"
#include "Character/CEnemy.h"
#include "CEnemy_Dash.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACEnemy_Dash : public ACEnemy
{
	GENERATED_BODY()

public: 
	ACEnemy_Dash(); 
	
	void BeginPlay() override; 

public: 
	virtual void Attack() override; 
	virtual void EndAttack() override; 
	
};
