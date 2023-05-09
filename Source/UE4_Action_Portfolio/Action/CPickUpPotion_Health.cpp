#include "Action/CPickUpPotion_Health.h"
#include "Global.h"
#include "Character/CPlayer.h"


void ACPickUpPotion_Health::OnOverlapBegin(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepREsult)
{
	if (Cast<ACPlayer>(otherActor) == nullptr)
		return;
	
	player->PickUpHealthPotion(30);
	
	UParticleSystem* Effect;
	CHelpers::GetAssetDynamic(&Effect, "ParticleSystem'/Game/Effects/P_Health_Potion.P_Health_Potion'");
	FTransform transfom; 
	transfom.AddToTranslation(player->GetActorLocation() + FVector(0,0,130.0f)); 

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, transfom); 

	Destroy();
}

void ACPickUpPotion_Health::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
