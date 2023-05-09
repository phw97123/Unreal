#include "CPickUpPotion.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

ACPickUpPotion::ACPickUpPotion()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere",Scene);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACPickUpPotion::OnOverlapBegin); 
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACPickUpPotion::OnOverlapEnd);
}

void ACPickUpPotion::BeginPlay()
{
	player = Cast<ACPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();

	Super::BeginPlay();
	
}

void ACPickUpPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

