#include "CAnimNotify_SpawnArrow.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Action/CActionData.h"
#include "Action/CDoAction.h"
#include "Component/CActionComponent.h"

FString UCAnimNotify_SpawnArrow::GetNotifyName_Implementation() const
{
	return "ArrowSpawn";
}

void UCAnimNotify_SpawnArrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action); 

	action->GetCurrent()->GetDoAction()->Spawn();
}
