#include "Interaction/CInteraction_StoneStatue.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"
#include "Action/CEnemySpawner.h"
#include "Character/CPlayer.h"

ACInteraction_StoneStatue::ACInteraction_StoneStatue()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box"); 
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh", Box);
	CHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Text", Mesh); 

	RootComponent = Cast<USceneComponent>(Box); 
}

void ACInteraction_StoneStatue::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACInteraction_StoneStatue::OnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACInteraction_StoneStatue::OnOverlapEnd);

	bIsPlayInside = false; 
}

void ACInteraction_StoneStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPlayInside && GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::G))
	{
		PlayLevelSequence(); 
	}
}

void ACInteraction_StoneStatue::PlayLevelSequence()
{
	if (LevelSequence)
	{
		ALevelSequenceActor* OutActor = nullptr; 
		ULevelSequencePlayer* LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, 
			FMovieSceneSequencePlaybackSettings(), OutActor); 

		LevelSequencePlayer->Play();
		if (LevelSequencePlayer)
		{
			LevelSequencePlayer->OnFinished.AddDynamic(this, &ACInteraction_StoneStatue::OnLevelSequencePlayerFinished);
		}

		Text->SetVisibility(false); 
	}
}

void ACInteraction_StoneStatue::OnLevelSequencePlayerFinished()
{
	SetActorLocation(FVector(-6220.0f, 5600.0f, 7290.0f));

	FindEnemySpawner();

	FTimerHandle WaitHandle; 
	float WaitTime = 3.0f; 

	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{

			ACPlayer* player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			player->VisibillityNumberOfEnemies();

		}), WaitTime, false);
}

void ACInteraction_StoneStatue::OnOverlapBegin(UPrimitiveComponent* selfComp, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepREsult)
{
	if (OtherActor && OtherActor != this && OtherActor->IsA(ACharacter::StaticClass()))
	{
		bIsPlayInside = true;

		Text->SetVisibility(true); 
	}
}

void ACInteraction_StoneStatue::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && OtherActor->IsA(ACharacter::StaticClass()))
	{
		bIsPlayInside = false;

		Text->SetVisibility(false);
	}
}

void ACInteraction_StoneStatue::FindEnemySpawner()
{
	TArray<AActor*> ActorsToFind; 

	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACEnemySpawner::StaticClass(), ActorsToFind);
	}

	for (AActor* EnemySpawners : ActorsToFind)
	{
		ACEnemySpawner* EnemySpawner = Cast<ACEnemySpawner>(EnemySpawners);
		if (EnemySpawner)
		{
			EnemySpawner->EffectActivate(); 

			//FTimerHandle TimerHandle; 
		
			EnemySpawner->SpawnEnemyAtChildActor();
		}
	}
}

