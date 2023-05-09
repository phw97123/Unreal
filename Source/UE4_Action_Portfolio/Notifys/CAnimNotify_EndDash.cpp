#include "CAnimNotify_EndDash.h"
#include "Global.h"
#include "Character/CPlayer.h"

FString UCAnimNotify_EndDash::GetNotifyName_Implementation() const
{
	return "EndDash";
}

void UCAnimNotify_EndDash::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner()); 

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner()); 
	CheckNull(player); 

	player->End_Dash();
	//CLog::Print("Dash"); 
}
