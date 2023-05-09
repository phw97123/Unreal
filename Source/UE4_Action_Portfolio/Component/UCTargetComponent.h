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

	/* ȭ����� ������ ��ġ�� ����Ʈ�� �����ϴ��� ��Ÿ���� �Լ�
	 * ScreenRatio ���� 0.0f�� �ƴ� ��� ����Ʈ �߾ӿ��� �ش� ���� ȭ��ũ���� ������ŭ�� ������ ���Ͱ� �����ϴ��� ��Ÿ��
	 * ActorScreenPosition : ȭ�鿡���� ���� ��ġ 
	 * ScreenRatio : ȭ�� ũ���� ������ ��Ÿ���� �� , 0.0f �� ��� ����Ʈ�� �ִ�ũ�⸦ ����ϸ� 0.2f �� ��� �߾ӿ��� ����Ʈ��
	 * �����¿츦 0.2f ������ŭ ���� ũ�⸦ ���
	*/
	bool IsInViewport(FVector2D ActorScreenPosition, float ScreenRatio = 0.0f) const; 

	/*���ڷ� ���� ������ ȭ�鿡���� ��ġ�� �ش� ���Ͱ� ȭ�鿡 �����ϴ����� ��ȯ�ϴ� �Լ�
	 * SerchActor ȭ�鿡���� ��ġ�� ã�� ����
	 * return Get<0> ���ڷ� ���� ������ ȭ�鿡���� ��ġ
	 * return Get<1> ���ڷ� ���� ���Ͱ� ȭ�鿡 �����ϴ����� ��Ÿ��
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
