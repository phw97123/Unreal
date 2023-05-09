#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CFeetComponent.generated.h"

USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()

public: 
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector LeftDistance; //X�ุ ���

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector RightDistance; //X�ุ ���

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector PelvisDistance; //Z�ุ ���

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator LeftRotation; 

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator RightRotation; 
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_ACTION_PORTFOLIO_API UCFeetComponent : public UActorComponent
{
	GENERATED_BODY()

protected: 
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InverseKinemetics")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InverseKinemetics")
		FName LeftSocket = "Foot_L"; 

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InverseKinemetics")
		FName RightSocket = "Foot_R"; 

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InverseKinemetics")
		float TraceDistance = 55; 

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InverseKinemetics")
		float InterpSpeed = 17; 

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InverseKinemetics")
		float OffsetDistance = 5; 

public: 
	FORCEINLINE	const FFeetData& GetData() { return Data; }

public:	
	UCFeetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private: 
	void Trace(FName InSocket, float& OutDistance, FRotator& OutRotation); 

private: 
	class ACharacter* OwnerCharacter; 

	FFeetData Data;
};
