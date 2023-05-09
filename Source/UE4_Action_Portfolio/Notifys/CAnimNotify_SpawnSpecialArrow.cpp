#include "Notifys/CAnimNotify_SpawnSpecialArrow.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Action/CActionData.h"
#include "Action/CDoSpecialAction.h"
#include "Component/CActionComponent.h"

FString UCAnimNotify_SpawnSpecialArrow::GetNotifyName_Implementation() const
{
	return "SpecialArrowSpawn";
}

void UCAnimNotify_SpawnSpecialArrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	action->GetCurrent()->GetDoSpecialAction()->Spawn();
}
