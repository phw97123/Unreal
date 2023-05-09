#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UCTargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_ACTION_PORTFOLIO_API UUCTargetComponent : public UActorComponent
{
	GENERATED_BODY()

private: 
	UPROPERTY(EditAnywhere)
		float TraceRadius = 2000.0f;

	UPROPERTY(EditAnywhere)
		float InteropSpeed = 2.5f; 

	UPROPERTY(EditAnywhere)
		TEnumAsByte<EDrawDebugTrace::Type> Debug; 

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Particle; 

public:	
	UUCTargetComponent();

protected:
	virtual void BeginPlay() override;

public: 
	void ToggleTarget(); 
	void ChangeTargetLeft(); 
	void ChangeTargetRight(); 

private: 
	void StartTargeting(); 
	void EndTargeting(); 

	void SetTraceTargets(); 
	void SetTarget(); 

	void ChangeTarget(bool InRight); 
	void ChangeCursor(class ACharacter* InTarget); 

	/* 화면상의 액터의 위치가 뷰포트에 존재하는지 나타내는 함수
	 * ScreenRatio 값이 0.0f이 아닐 경우 뷰포트 중앙에서 해당 값의 화면크기의 비율만큼의 범위에 액터가 존재하는지 나타냄
	 * ActorScreenPosition : 화면에서의 액터 위치 
	 * ScreenRatio : 화면 크기의 비율을 나타내는 값 , 0.0f 일 경우 뷰포트의 최대크기를 사용하며 0.2f 일 경우 중앙에서 뷰포트의
	 * 상하좌우를 0.2f 비율만큼 줄인 크기를 사용
	*/
	bool IsInViewport(FVector2D ActorScreenPosition, float ScreenRatio = 0.0f) const; 

	/*인자로 받은 액터의 화면에서의 위치와 해당 액터가 화면에 존재하는지를 반환하는 함수
	 * SerchActor 화면에서의 위치를 찾을 액터
	 * return Get<0> 인자로 받은 액터의 화면에서의 위치
	 * return Get<1> 인자로 받은 액터가 화면에 존재하는지를 나타냄
	*/
	TTuple<FVector2D, bool> GetScreenPositionOfActor(AActor* SearchActor) const;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private: 
	class ACharacter* OwnerCharacter; 
	class ACharacter* Target; 
	class APlayerController* PlayerController;
	class UParticleSystemComponent* Attached; 
	
	TArray<class ACharacter*> TraceTargets;
		
};
