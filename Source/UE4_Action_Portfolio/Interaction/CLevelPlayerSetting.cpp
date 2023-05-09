#include "Interaction/CLevelPlayerSetting.h"
#include "Global.h"
#include "Character/CPlayer.h"
#include "CBossStagePotal.h"
#include "Components/Boxcomponent.h"
#include "Widget/CUserWidget_SkillSlot.h"

ACLevelPlayerSetting::ACLevelPlayerSetting()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box",RootComponent);

	Box->SetCollisionProfileName(TEXT("OverlapOnlyPawn")); 
}

void ACLevelPlayerSetting::NotifyActorBeginOverlap(AActor* OtherActor)
{
	ACPlayer* player = Cast<ACPlayer>(OtherActor);
	if (player)
	{ 
		player->Action->SetMode(EActionType::Sword); 
		Destroy(); 
	}
}

void ACLevelPlayerSetting::BeginPlay()
{
	Super::BeginPlay();
}

void ACLevelPlayerSetting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

