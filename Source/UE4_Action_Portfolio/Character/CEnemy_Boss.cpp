#include "Character/CEnemy_Boss.h"
#include "Global.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "CAIController_Enemy_Boss.h"
#include "Component/CActionComponent.h"
#include "Component/CStatusComponent.h"
#include "Component/CMontageComponent.h"
#include "Action/CDoAction.h"
#include "Action/CDoAction_BossAttack.h"
#include "Widget/CUserWidget_Health.h"
#include "Components/WidgetComponent.h"
#include "CPlayer.h"
#include "Action/CThrow.h"
#include "Action/CBossFloorPattern.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

ACEnemy_Boss::ACEnemy_Boss()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ACAIController_Enemy_Boss::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	USkeletalMesh* mesh; 
	CHelpers::GetAsset(&mesh, "SkeletalMesh'/Game/UndeadPack/Lich/Mesh/SK_Lich_Full.SK_Lich_Full'");
	GetMesh()->SetSkeletalMesh(mesh); 

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass(&animInstance, "AnimBlueprint'/Game/Enemy/Enemy_Boss/ABPC_Enemy_Boss.ABPC_Enemy_Boss_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	CHelpers::GetClass<UCUserWidget_Health>(&healthClass, "WidgetBlueprint'/Game/Widget/WB_BossHealthBar.WB_BossHealthBar_C'");

	HealthWidget->SetWidgetClass(healthClass);
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidget->SetWorldLocation(FVector(0, 0, 300)); 
	HealthWidget->SetDrawSize(FVector2D(140, 15));

	HealthWidget->SetVisibility(false); 
}

void ACEnemy_Boss::BeginPlay()
{
	Super::BeginPlay();


	UMaterialInstanceConstant* body;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/UndeadPack/Lich/Materials/MI_Lich_1.MI_Lich_1'");

	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);

	GetMesh()->SetMaterial(0, BodyMaterial);

	BossHealthBar = CreateWidget<UCUserWidget_Health>(GetWorld(), healthClass); 
	BossHealthBar->AddToViewport(); 

	playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); 

	Action->SetSwordMode();
}

void ACEnemy_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 

	BossHealthBar->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth()); 

	
	if ((Status->GetHealth() > 250 && Status->GetHealth() <= 300) || (Status->GetHealth() > 100 && Status->GetHealth() <= 150))
	{
		GroggyEffect->SetActive(true);
	}
	else
	{
		GroggyEffect->SetActive(false);
	}
}

void ACEnemy_Boss::Attack()
{
	CheckFalse(State->IsIdleMode()); 

	State->SetDashMode(); 
}

void ACEnemy_Boss::DanmakuAttack()
{
	//Action->DoAction(); 

	FVector bossLocation = GetActorLocation();
	FRotator bossRotation = GetActorRotation();

	// Spawn projectiles in a cone in front of the boss
	for (int i = 0; i < 3; i++)
	{
		FRotator projectileRotation = bossRotation + FRotator(0, i * 10, 0);
		FVector projectileLocation = bossLocation + (projectileRotation.Vector() *10);

		ACThrow* projectile = GetWorld()->SpawnActor<ACThrow>(FireballClass, projectileLocation, projectileRotation);
		if (projectile)
		{
			projectile->OnThrowBeginOverlap.AddDynamic(this, &ACEnemy_Boss::OnThrowBeginOverlap); 
			projectile->SetOwner(this);
		}
	}
}

void ACEnemy_Boss::EndAttack()
{
	OnAttackEnd.Broadcast(); 
}

void ACEnemy_Boss::TimerCallAttack()
{
	Action->DoAction();
}

void ACEnemy_Boss::BossFloorAttack()
{
	FVector TargetLocation = playerPawn->GetActorLocation(); 
	FRotator TargetRotaion = FRotator(0, 0, 0);

	ACBossFloorPattern* FloorParttern = GetWorld()->SpawnActor<ACBossFloorPattern>(FloorClass, TargetLocation, TargetRotaion);

	if (FloorParttern)
	{
		FloorParttern->OnFloorBeginOverlap.AddDynamic(this, &ACEnemy_Boss::OnFloorBeginOverlap);
		FloorParttern->SetOwner(this); 
	}
}

void ACEnemy_Boss::PlayDash()
{
	FVector Location = GetActorLocation();
	FVector PlayerLocation = playerPawn->GetActorLocation();
	FVector DashDirection = (PlayerLocation - Location).GetSafeNormal();

	float DistanceToPlayer = FVector::Distance(PlayerLocation, Location) - 150.0f;

	FVector DashLocation = Location + DashDirection * DistanceToPlayer;

	SetActorLocation(DashLocation);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACEnemy_Boss::TimerCallAttack, 0.2f, false);
}

void ACEnemy_Boss::OnThrowBeginOverlap(FHitResult InHItResult)
{
	FDamageEvent e;
	InHItResult.GetActor()->TakeDamage(10, e,UGameplayStatics::GetPlayerController(GetWorld(),0) , this);
}

void ACEnemy_Boss::OnFloorBeginOverlap(FHitResult InHItResult)
{
	FDamageEvent e;
	InHItResult.GetActor()->TakeDamage(15, e, UGameplayStatics::GetPlayerController(GetWorld(), 0), this);

	//CLog::Print("TrueDamageFloor"); 
}

void ACEnemy_Boss::Dash()
{
	Montages->PlayDash(); 
}
