#include "Action/CEnemySpawner.h"
#include "Global.h"
#include "Character/CEnemy.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SceneComponent.h"


ACEnemySpawner::ACEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &SpawnEffect, "SpawnEffect",Scene); 

}

void ACEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnEffect->bAutoActivate = false;
}

void ACEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACEnemySpawner::SpawnEnemyAtChildActor()
{
	if (EnemyClass)
	{
		FActorSpawnParameters SpawnParms; 
		SpawnParms.Owner = this; 
		SpawnParms.Instigator = GetInstigator();

		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		ACEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ACEnemy>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParms);

		Destroy();
	}
}

void ACEnemySpawner::EffectActivate()
{
	SpawnEffect->Activate(); 
}
