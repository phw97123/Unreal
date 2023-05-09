#include "Action/CThrow.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACThrow::ACThrow()
{
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere"); 
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Sphere); 
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile"); 

	InitialLifeSpan = 3.0f; 

	Projectile->InitialSpeed = 1500.0f; 
	Projectile->MaxSpeed = 1500.0f; 
	Projectile->ProjectileGravityScale = 0.0f; 
}

void ACThrow::BeginPlay()
{
	Super::BeginPlay();

	TArray<USphereComponent*> components;
	GetComponents<USphereComponent>(components);

	for (USphereComponent* sphere : components)
		sphere->OnComponentBeginOverlap.AddDynamic(this, &ACThrow::OnComponentBeginOverlap);


}

void ACThrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (!!Explosion)
	//{
	//	FTransform transform = ExplosionTransform;
	//	transform.AddToTranslation(SweepResult.Location);
	//	transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));

	//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, transform);
	//}

	if (OnThrowBeginOverlap.IsBound())
		OnThrowBeginOverlap.Broadcast(SweepResult);
}

