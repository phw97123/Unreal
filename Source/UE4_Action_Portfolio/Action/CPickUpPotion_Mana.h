#pragma once

#include "CoreMinimal.h"
#include "Action/CPickUpPotion.h"
#include "CPickUpPotion_Mana.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACPickUpPotion_Mana : public ACPickUpPotion
{
	GENERATED_BODY()
	
public:
	virtual	void OnOverlapBegin(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepREsult) override;

	virtual	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:
	float mana = 40.0f;
};
