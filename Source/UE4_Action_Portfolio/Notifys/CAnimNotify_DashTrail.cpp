#include "Notifys/CAnimNotify_DashTrail.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Misc/OutputDeviceNull.h"

FString UCAnimNotify_DashTrail::GetNotifyName_Implementation() const
{
	return "DashTrail";
}

void UCAnimNotify_DashTrail::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation); 

	CheckNull(MeshComp); 
	CheckNull(MeshComp->GetOwner()); 

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner()); 
	CheckNull(player); 

	FOutputDeviceNull par;
	player->CallFunctionByNameWithArguments(TEXT("GhostTrail"), par, nullptr, true);

}
