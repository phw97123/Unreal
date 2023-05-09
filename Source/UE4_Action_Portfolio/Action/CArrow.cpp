#include "Action/CArrow.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACArrow::ACArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh", Sphere); 
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &HeavyActionEffect, "HeavyActionEffect", Mesh);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &ArrowEffect, "ArrowEffect", Mesh);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &SpecialActionEffect, "SpecialActionEffect", Mesh);
	CHelpers::CreateComponent<USphereComponent>(this, &SpecialActionCollision, "SpecialActionCollision", SpecialActionEffect);

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	InitialLifeSpan = 3.0f;
	
	HeavyActionEffect->bAutoActivate = false;
	ArrowEffect->bAutoActivate = false;
	SpecialActionEffect->bAutoActivate = false;

	Projectile->InitialSpeed = 3000.0f; 
	Projectile->MaxSpeed = 3000.0f; 
	Projectile->ProjectileGravityScale = 0.0f;
	Projectile->bAutoActivate = false;
	//Projectile->SetUpdatedComponent(Mesh); 
}

void ACArrow::BeginPlay()
{
	Super::BeginPlay();

	TArray<USphereComponent*> components;
	GetComponents<USphereComponent>(components); 
	
	//Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	for (USphereComponent* sphere : components)
	{
		sphere->OnComponentBeginOverlap.AddDynamic(this, &ACArrow::OnComponentBeginOverlap);
		sphere->OnComponentBeginOverlap.AddDynamic(this, &ACArrow::OnSpecialActionBeginOverlap);
	}
	
}

void ACArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACArrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OnArrowBeginOverlap.IsBound())
		OnArrowBeginOverlap.Broadcast(SweepResult); 
}

void ACArrow::OnSpecialActionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OnEffectBeginOverlap.IsBound())
		OnEffectBeginOverlap.Broadcast(SweepResult);
}


void ACArrow::Launch()
{
	Projectile->Activate();
	ArrowEffect->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	ArrowEffect->Activate();
}

void ACArrow::SpecialLaunch()
{
	Projectile->Activate();
	ArrowEffect->SetWorldScale3D(FVector(5.0f, 5.0f, 5.0f)); 
	ArrowEffect->Activate();
	SpecialActionEffect->Activate();
}

void ACArrow::HeavyAction()
{
	HeavyActionEffect->Activate();
}

void ACArrow::OnSpecialCollision()
{
	SpecialActionCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACArrow::OffSpecialCollision()
{
	SpecialActionCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
}

