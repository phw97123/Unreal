#include "CPickUp.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Character/CPlayer.h"
#include "CAttachment.h"
#include "Particles/ParticleSystem.h"

ACPickUp::ACPickUp()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	
	RootComponent = Cast<USceneComponent>(Sphere);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACPickUp::OnOverlapBegin);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACPickUp::OnOverlapEnd);
}

void ACPickUp::BeginPlay()
{
	player = Cast<ACPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();

	Super::BeginPlay();
}

void ACPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPickUp::OnOverlapBegin(UPrimitiveComponent* selfComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepREsult)
{
	if (Cast<ACPlayer>(otherActor) == nullptr)
		return;

	player->SetbItemOverlap(true); 
	player->SetWeaponType((int)Type); 

	FTransform transform;
	transform.SetLocation(GetActorLocation());
}

void ACPickUp::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	player->SetbItemOverlap(false);
	player->SetPickUp(false);
}


