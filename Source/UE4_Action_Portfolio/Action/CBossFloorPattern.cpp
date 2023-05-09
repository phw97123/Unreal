#include "Action/CBossFloorPattern.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SceneComponent.h"

ACBossFloorPattern::ACBossFloorPattern()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<UDecalComponent>(this, &Decal, "Decal",Scene); 
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere", Scene);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Effect, "Effect", Scene);

	Effect->bAutoActivate = false;
}

void ACBossFloorPattern::BeginPlay()
{
	Super::BeginPlay();

	TArray<USphereComponent*> components;
	GetComponents<USphereComponent>(components);

	for (USphereComponent* sphere : components)
		sphere->OnComponentBeginOverlap.AddDynamic(this, &ACBossFloorPattern::OnComponentBeginOverlap);

	FTimerDelegate TimerDelegate = FTimerDelegate::CreateLambda([&]()
		{
			Decal->SetVisibility(false);
			Effect->Activate(true);

			Sphere->SetRelativeLocation(FVector(0,0,100)); 

			FTimerDelegate TimerDelegate1 = FTimerDelegate::CreateLambda([&]()
				{
					Destroy();
				});

			GetWorldTimerManager().SetTimer(DelayTimerHandle, TimerDelegate1, 0.3f, false);
		});

	GetWorldTimerManager().SetTimer(DelayTimerHandle, TimerDelegate, 0.5f, false);
}

void ACBossFloorPattern::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACBossFloorPattern::StartDelay(float DelayTime)
{
	
}

void ACBossFloorPattern::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (!!Explosion)
	//{
	//	FTransform transform = ExplosionTransform;
	//	transform.AddToTranslation(SweepResult.Location);
	//	transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));

	//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, transform);
	//}


	if (OnFloorBeginOverlap.IsBound())
		OnFloorBeginOverlap.Broadcast(SweepResult);
}