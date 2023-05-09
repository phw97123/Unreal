#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPickUp.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Unarmed, Sword, Bow, Fist, Max,
};

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACPickUp : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item")
		class UParticleSystem* Particle;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item")
		class AActor* Item;
		
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Item")
		EWeaponType Type;
public:	
	ACPickUp();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepREsult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public: 
	class ACPlayer* player;

	int32 GetWeaponType() { return (int32)Type; }
};
