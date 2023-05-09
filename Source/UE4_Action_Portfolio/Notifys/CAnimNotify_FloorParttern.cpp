#include "Notifys/CAnimNotify_FloorParttern.h"
#include "Global.h"
#include "Character/CEnemy_Boss.h"

FString UCAnimNotify_FloorParttern::GetNotifyName_Implementation() const
{
	return "BossFloorAttack";
}

void UCAnimNotify_FloorParttern::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp); 
	CheckNull(MeshComp->GetOwner()); 

	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(MeshComp->GetOwner()); 
	CheckNull(boss); 

	boss->BossFloorAttack(); 
}
