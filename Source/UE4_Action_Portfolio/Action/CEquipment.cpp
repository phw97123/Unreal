#include "Action/CEquipment.h"
#include "Global.h"
#include "Component/CStateComponent.h"
#include "Component/CStatusComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/CCharacter_Interface.h"

ACEquipment::ACEquipment()
{
}

void ACEquipment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner()); 
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter); 
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter); 


	Super::BeginPlay();
	
}

void ACEquipment::Equip_Implementation()
{
	State->SetEquipMode(); 

	if (Data.AnimMontage != NULL)
		OwnerCharacter->PlayAnimMontage(Data.AnimMontage, Data.PlayRatio, Data.StartSection);
	else
		End_Equip(); 

	if (Data.bPawnControl == true)
	{
		OwnerCharacter->bUseControllerRotationYaw = true; 
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	ICCharacter_Interface* character = Cast<ICCharacter_Interface>(OwnerCharacter); 
	CheckNull(character); 

	character->ChangeColor(Color); 
}

void ACEquipment:: Begin_Equip_Implementation()
{
	if (OnEquipmentDelegate.IsBound())
		OnEquipmentDelegate.Broadcast();
}

void ACEquipment::End_Equip_Implementation()
{
	bEquipped = true;

	State->SetIdleMode(); 
}

void ACEquipment::Unequip_Implementation()
{
	bEquipped = false;

	if (OnUnequipmentDelegate.IsBound())
		OnUnequipmentDelegate.Broadcast();

	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}
