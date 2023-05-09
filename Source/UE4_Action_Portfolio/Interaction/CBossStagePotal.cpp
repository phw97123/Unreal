#include "Interaction/CBossStagePotal.h"
#include "Global.h"
#include "Components/Boxcomponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Character/CEnemy_Boss.h"
#include "Widget/CUserWidget_LoadingScreen.h"

ACBossStagePotal::ACBossStagePotal()
{
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &PotalEffect, "PotalEffect");
	CHelpers::CreateComponent<UBoxComponent>(this, &TriggerVolume, "TriggerVolume", PotalEffect);
	
	RootComponent = PotalEffect; 

	TriggerVolume->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

void ACBossStagePotal::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACBossStagePotal::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor); 

	if (OtherActor && OtherActor->IsA(APawn::StaticClass()))
	{
		APawn* PlayerPawn = Cast<APawn>(OtherActor);
		if (PlayerPawn)
		{
			TSubclassOf<UCUserWidget_LoadingScreen> WidgetClass = LoadClass<UCUserWidget_LoadingScreen>(nullptr, TEXT("WidgetBlueprint'/Game/Widget/WB_LoadingScreen.WB_LoadingScreen_C'"));

			Loading = CreateWidget<UCUserWidget_LoadingScreen>(GetWorld(), WidgetClass); 
			if (Loading)
			{
				Loading->AddToViewport(); 
			}

			FVector NewLocation = FVector(-330.0f,4810.0f, 7838.0f);
			//(X=-330.000000,Y=4810.000000,Z=7838.000000)

			//(X=-241.097366,Y=7049.999512,Z=7838.582520)
			PlayerPawn->SetActorLocation(NewLocation);

			FTimerHandle WaitHandle1;
			float WaitTime1 = 2.0f;

			FTimerHandle WaitHandle2;
			float WaitTime2 = 2.0f;

			GetWorld()->GetTimerManager().SetTimer(WaitHandle1, this, &ACBossStagePotal::ReMoveWidget, WaitTime1, false);
			GetWorld()->GetTimerManager().SetTimer(WaitHandle2, this, &ACBossStagePotal::SpawnBossMonster, WaitTime2, false);
			
		}
	}
}

void ACBossStagePotal::SpawnBossMonster()
{
	if (BossClass)
	{
		FActorSpawnParameters SpawnParams; 
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		FTransform BossTransform; 
		BossTransform.SetLocation(FVector(-241.0f, 7050.0f, 7838.0f));
		BossTransform.SetRotation(FQuat(FRotator(0.0f, -90.0f, 0.0f))); 

		ACEnemy_Boss* Boss = GetWorld()->SpawnActor<ACEnemy_Boss>(BossClass,BossTransform, SpawnParams);
	}
}

void ACBossStagePotal::ReMoveWidget()
{
	Loading->RemoveFromParent();
}

