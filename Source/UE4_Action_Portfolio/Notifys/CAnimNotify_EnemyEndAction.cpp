#include "Notifys/CAnimNotify_EnemyEndAction.h"
#include "Global.h"
#include "Character/CEnemy.h"

FString UCAnimNotify_EnemyEndAction::GetNotifyName_Implementation() const
{
	return "EnemyEndAction";
}

void UCAnimNotify_EnemyEndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy* enemy = Cast<ACEnemy>(MeshComp->GetOwner()); 
	CheckNull(enemy); 

	enemy->EndAttack(); 
}