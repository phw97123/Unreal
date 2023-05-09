#include "Notifys/CAnimNotify_BossDash.h"
#include "Global.h"
#include "Character/CEnemy_Boss.h"


FString UCAnimNotify_BossDash::GetNotifyName_Implementation() const
{
	return "BossDash";
}

void UCAnimNotify_BossDash::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(MeshComp->GetOwner()); 
	CheckNull(boss); 

	boss->PlayDash(); 
}

