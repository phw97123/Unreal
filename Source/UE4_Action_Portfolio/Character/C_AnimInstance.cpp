#include "C_AnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"

void UC_AnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner()); 
	CheckNull(character);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);
	CheckNull(action);

	action->OnActionTypeChanged.AddDynamic(this, &UC_AnimInstance::OnActionTypeChanged);
}

void UC_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);

	Speed = character->GetVelocity().Size2D();
	//CalculateDirection : 속도와 회전 포워드 벡터 사이의 각도를 반환 
	//입력값: 표적 애님인스턴스 객체 Ref(지금은 self), 속도 vector, 기본 회전 rotator
	Direction = CalculateDirection(character->GetVelocity(), character->GetControlRotation());

	UCFeetComponent* feet = CHelpers::GetComponent<UCFeetComponent>(character); 
	if (!!feet)
		FeetData = feet->GetData();

}

void UC_AnimInstance::OnActionTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	ActionType = InNewType;
}