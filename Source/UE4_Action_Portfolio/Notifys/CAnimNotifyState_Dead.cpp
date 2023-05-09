#include "CAnimNotifyState_Dead.h"
#include "Global.h"
#include "Character/CCharacter_Interface.h"

FString UCAnimNotifyState_Dead::GetNotifyName_Implementation() const
{
	return "Dead"; 
}

void UCAnimNotifyState_Dead::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration); 
	CheckNull(MeshComp); 
	CheckNull(MeshComp->GetOwner()); 

	ICCharacter_Interface* character = Cast<ICCharacter_Interface>(MeshComp->GetOwner()); 
	CheckNull(character); 
	
	character->Begin_Dead(); 
}

void UCAnimNotifyState_Dead::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation); 
	CheckNull(MeshComp); 
	CheckNull(MeshComp->GetOwner()); 

	ICCharacter_Interface* character = Cast<ICCharacter_Interface>(MeshComp->GetOwner()); 
	CheckNull(character); 

	character->End_Dead(); 
}
