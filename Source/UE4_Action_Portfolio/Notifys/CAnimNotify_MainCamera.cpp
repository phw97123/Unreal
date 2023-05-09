#include "Notifys/CAnimNotify_MainCamera.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "Camera/CameraComponent.h"

FString UCAnimNotify_MainCamera::GetNotifyName_Implementation() const
{
	return "MainCamera";
}

void UCAnimNotify_MainCamera::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->OffSpringArmRotation();
	player->SpringArmChange(FRotator(0, 90, 0), 700);

	UGameplayStatics::GetPlayerController(player, 0)->SetViewTargetWithBlend(player, 0.2, EViewTargetBlendFunction::VTBlend_Linear, 1.0);
}




