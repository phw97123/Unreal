#include "Notifys/CAnimNotify_EndAction.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Action/CActionData.h"
#include "Action/CDoAction.h"
#include "Component/CActionComponent.h"


FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "EndAction";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation); 
	CheckNull(MeshComp); 
	CheckNull(MeshComp->GetOwner()); 

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner()); 
	CheckNull(action); 

	action->GetCurrent()->GetDoAction()->End_DoAction(); 
}
