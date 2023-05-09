#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CCharacter_Interface.generated.h"

UINTERFACE(MinimalAPI)
class UCCharacter_Interface : public UInterface
{
	GENERATED_BODY()
};

class UE4_ACTION_PORTFOLIO_API ICCharacter_Interface
{
	GENERATED_BODY()

public:
	virtual void ChangeColor(FLinearColor InColor) = 0; 

	virtual void Begin_Dead() {}
	virtual void End_Dead() {}

};
 