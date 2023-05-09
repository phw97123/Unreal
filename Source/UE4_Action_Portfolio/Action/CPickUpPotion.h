#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPickUpPotion.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACPickUpPotion : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USphereComponent* Sphere;

public:	
	ACPickUpPotion();

protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		virtual	void OnOverlapBegin(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepREsult) {}

	UFUNCTION()
		virtual	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {}

public:
	class ACPlayer* player;

};
