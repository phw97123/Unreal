#include "Notifys/CAnimNotify_BeginAction.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Action/CActionData.h"
#include "Action/CDoAction.h"
#include "Component/CActionComponent.h"

FString UCAnimNotify_BeginAction::GetNotifyName_Implementation() const
{
	return "Begin Action";
}

void UCAnimNotify_BeginAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp); 
	CheckNull(MeshComp->GetOwner()); 

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner()); 
	CheckNull(action); 

	action->GetCurrent()->GetDoAction()->Begin_DoAction(); 
}
