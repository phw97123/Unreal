#include "CAnimNotify_SpecialBegin_Action.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Action/CActionData.h"
#include "Action/CDoSpecialAction.h"
#include "Component/CActionComponent.h"

FString UCAnimNotify_SpecialBegin_Action::GetNotifyName_Implementation() const
{
	return "SpecialBegin_Action"; 
}

void UCAnimNotify_SpecialBegin_Action::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp,Animation); 
	CheckNull(MeshComp); 
	CheckNull(MeshComp->GetOwner()); 

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action); 

	action->GetCurrent()->GetDoSpecialAction()->Begin_DoSpecialAction(); 

}
