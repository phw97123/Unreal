#include "Notifys/CAnimNotify_HeavyAttack.h"
#include "Global.h"
#include "Action/CActionData.h"
#include "Action/CDoAction.h"
#include  "Component/CActionComponent.h"

FString UCAnimNotify_HeavyAttack::GetNotifyName_Implementation() const
{
	return "HeavyAttack";
}

void UCAnimNotify_HeavyAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp); 
	CheckNull(MeshComp->GetOwner()); 

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner()); 
	CheckNull(action); 

	action->GetCurrent()->GetDoAction()->HeavyAction(); 
}
