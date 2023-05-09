#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CInteraction_StoneStatue.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACInteraction_StoneStatue : public AActor
{
	GENERATED_BODY()

public: 
	UPROPERTY(VisibleDefaultsOnly)
		class UBoxComponent* Box; 

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Text")
		class UTextRenderComponent* Text; 

	UPROPERTY(EditAnywhere, Category = "Level Sequence")
		class ULevelSequence* LevelSequence;

public:	
	ACInteraction_StoneStatue();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public: 
	UFUNCTION()
		void PlayLevelSequence(); 

	UFUNCTION()
		void OnLevelSequencePlayerFinished(); 
public:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* selfComp, class AActor* OtherActor, UPrimitiveComponent* otherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepREsult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void FindEnemySpawner(); 
private: 
	bool bIsPlayInside;
};
