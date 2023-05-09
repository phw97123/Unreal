#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CWeapon_Interface.generated.h"

UINTERFACE(MinimalAPI)
class UCWeapon_Interface : public UInterface
{
	GENERATED_BODY()
};

class UE4_ACTION_PORTFOLIO_API ICWeapon_Interface
{
	GENERATED_BODY()

public:
	virtual void GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection) = 0;
};
