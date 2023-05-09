#include "Action/CPickUpPotion_Mana.h"
#include "Global.h"
#include "Character/CPlayer.h"

void ACPickUpPotion_Mana::OnOverlapBegin(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepREsult)
{
	if (Cast<ACPlayer>(otherActor) == nullptr)
		return;

	player->PickUpManaPotion(mana); 

	UParticleSystem* Effect;
	CHelpers::GetAssetDynamic(&Effect, "ParticleSystem'/Game/Effects/P_Mana_Potion.P_Mana_Potion'");
	FTransform transfom;
	transfom.AddToTranslation(player->GetActorLocation() + FVector(0, 0, 130.0f));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, transfom);

	Destroy();
}

void ACPickUpPotion_Mana::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
