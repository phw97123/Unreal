#include "Component/CActionComponent.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Action/CAttachment.h"
#include "Action/CEquipment.h"
#include "Action/CActionData.h"
#include "Action/CDoAction.h"
#include "Action/CDoSpecialAction.h"
#include "GameFramework/Character.h"

UCActionComponent::UCActionComponent()
{
}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* character = Cast<ACharacter>(GetOwner()); 
	
	player = Cast<ACPlayer>(GetOwner()); 

	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!Datas[i])
			Datas[i]->BeginPlay(character);
	}
}

void UCActionComponent::SetUnarmedMode()
{
	if (!!Datas[(int32)Type])
	{
		ACEquipment* equipment = Datas[(int32)Type]->GetEquipment();
		CheckNull(equipment);

		equipment->Unequip();
	}

	ChangeType(EActionType::Unarmed);
}

void UCActionComponent::SetSwordMode()
{
	SetMode(EActionType::Sword);
}

void UCActionComponent::SetBowMode()
{
	SetMode(EActionType::Bow);
}

void UCActionComponent::SetFistMode()
{
	SetMode(EActionType::Fist);
}

void UCActionComponent::OffAllCollision()
{
	for (UCActionData* data : Datas)
	{
		if (!!data == false)
			continue;

		if (!!data->GetAttachment() == false)
			continue; 

		data->GetAttachment()->OffCollision();
	}
}

void UCActionComponent::DoAction()
{
	CheckTrue(IsUnarmedMode());

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetDoAction(); 

		if (!!action)
			action->DoAction();
	}
}

void UCActionComponent::DoSpecialAction()
{
	CheckTrue(IsUnarmedMode()); 

	if (!!Datas[(int32)Type])
	{
		ACDoSpecialAction* action = Datas[(int32)Type]->GetDoSpecialAction();

		if (!!action)
			action->DoSpecialAction();
	}
}

void UCActionComponent::DoAim()
{
	SetAimMode(true); 
}

void UCActionComponent::UnDoAim()
{
	SetAimMode(false);
}

void UCActionComponent::SetAimMode(bool InAim)
{
	CheckTrue(IsUnarmedMode()); 

	if (!!Datas[(int32)Type])
	{
		ACDoAction* action = Datas[(int32)Type]->GetDoAction(); 

		if (!!action)
			InAim ? action->OnAim() : action->OffAim();
	}
}

void UCActionComponent::SetMode(EActionType InType)
{
	if (Type == InType) // 같은무기 해제
	{
		SetUnarmedMode(); 
		player->EquipSkillSlot(); 

		return; 
	}
	else if (IsUnarmedMode() == false) // 다른 무기 장착
	{
		ACEquipment* equipment = Datas[(int32)Type]->GetEquipment(); 
		CheckNull(equipment); 

		equipment->Unequip(); 
	}

	if (!!Datas[(int32)InType])
	{
		ACEquipment* equipment = Datas[(int32)InType]->GetEquipment();
		CheckNull(equipment);

		equipment->Equip();
	}

	ChangeType(InType);
}

void UCActionComponent::ChangeType(EActionType InNewType)
{
	EActionType prevType = InNewType; 
	Type = InNewType; 

	if(OnActionTypeChanged.IsBound())
		OnActionTypeChanged.Broadcast(prevType, InNewType);
}
