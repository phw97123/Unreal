#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CArrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTArrowBeginOverlap, FHitResult, InHitResult);

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACArrow : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh; 

	UPROPERTY(VisibleDefaultsOnly)
		 class UParticleSystemComponent* HeavyActionEffect;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* ArrowEffect;

	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* SpecialActionEffect;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* SpecialActionCollision;


public: 
	FORCEINLINE	UStaticMeshComponent* GetMesh() { return Mesh; }

	ACArrow();

protected:
	virtual void BeginPlay() override; 
public:	
	virtual void Tick(float DeltaTime) override;

private: 
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnSpecialActionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public: 
	UPROPERTY(BlueprintAssignable)
		FTArrowBeginOverlap OnArrowBeginOverlap;

	UPROPERTY(BlueprintAssignable)
		FTArrowBeginOverlap OnEffectBeginOverlap;

public: 
	UFUNCTION()
		void Launch();

	UFUNCTION()
		void SpecialLaunch();

	UFUNCTION()
		void HeavyAction();

public: 
	void OnSpecialCollision(); 
	void OffSpecialCollision(); 
};
