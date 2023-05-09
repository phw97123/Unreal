#include "Notifys/CAnimNotify_BossThrowAttack.h"
#include "Global.h"
#include "Character/CEnemy_Boss.h"

FString UCAnimNotify_BossThrowAttack::GetNotifyName_Implementation() const
{
	return "BossThrow"; 
}

void UCAnimNotify_BossThrowAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy_Boss* boss = Cast<ACEnemy_Boss>(MeshComp->GetOwner());
	CheckNull(boss);
	
	boss->DanmakuAttack();
}
