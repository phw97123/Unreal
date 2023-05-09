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
	//CalculateDirection : �ӵ��� ȸ�� ������ ���� ������ ������ ��ȯ 
	//�Է°�: ǥ�� �ִ��ν��Ͻ� ��ü Ref(������ self), �ӵ� vector, �⺻ ȸ�� rotator
	Direction = CalculateDirection(character->GetVelocity(), character->GetControlRotation());

	UCFeetComponent* feet = CHelpers::GetComponent<UCFeetComponent>(character); 
	if (!!feet)
		FeetData = feet->GetData();

}

void UC_AnimInstance::OnActionTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	ActionType = InNewType;
}