#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Action/CActionData.h"
#include "CDoSpecialAction.generated.h"

UCLASS()
class UE4_ACTION_PORTFOLIO_API ACDoSpecialAction : public AActor
{
	GENERATED_BODY()

public: 
	FORCEINLINE void SetDatas(TArray<FDoActionData> InDatas) { Datas = InDatas; }	
	FORCEINLINE void SetEquipped(const bool* InEquipped) { bEquipped = InEquipped; }
public:
	ACDoSpecialAction();

public:
	UFUNCTION()
	virtual void DoSpecialAction() {}

	UFUNCTION()
	virtual void Begin_DoSpecialAction() {}

	UFUNCTION()
	virtual void End_DoSpecialAction() {}

	UFUNCTION()
	virtual void Spawn() {}

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) {}

	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) {}

	UFUNCTION()
		virtual void OnAttachmentCollision() {}
	UFUNCTION()
		virtual void OffAttachmentCollision() {}

protected: 
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter; 
	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State; 
	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

protected: 
	const bool* bEquipped; 
	TArray<FDoActionData> Datas;
};
