#include "CEnemy.h"
#include "Global.h"
#include "Action/CActionData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Component/CActionComponent.h"
#include "Component/CMontageComponent.h"
#include "Component/CStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "Widget/CUserWidget_Health.h"
#include "Widget/CUserWidget_DamageNumber.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh()); 
	CHelpers::CreateComponent<UWidgetComponent>(this, &DamageNumber, "DamageNumber", GetMesh());

	CHelpers::CreateComponent<UParticleSystemComponent>(this, &GroggyEffect, "GroggyEffect", GetMesh());

	CHelpers::CreateActorComponent<UCActionComponent>(this, &Action, "Action"); 
	CHelpers::CreateActorComponent<UCMontageComponent>(this, &Montages, "Montages"); 
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State"); 
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status"); 

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90)); 
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0)); 

	USkeletalMesh* mesh;
	CHelpers::GetAsset(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh); 

	TSubclassOf<UAnimInstance> animInstance; 
	CHelpers::GetClass(&animInstance, "AnimBlueprint'/Game/Enemy/ABPC_Enemy.ABPC_Enemy_C'");
	GetMesh()->SetAnimInstanceClass(animInstance); 

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0); 

	TSubclassOf<UCUserWidget_Health> healthClass;
	CHelpers::GetClass<UCUserWidget_Health>(&healthClass, "WidgetBlueprint'/Game/Widget/WB_EnemyHealth.WB_EnemyHealth_C'");
	HealthWidget->SetWidgetClass(healthClass); 
	HealthWidget->SetRelativeLocation(FVector(0, 0, 205)); 
	HealthWidget->SetDrawSize(FVector2D(140, 15)); 
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen); 

	TSubclassOf<UCUserWidget_DamageNumber> damageNumberClass;
	CHelpers::GetClass<UCUserWidget_DamageNumber>(&damageNumberClass, "WidgetBlueprint'/Game/Widget/WB_DamageNumber.WB_DamageNumber_C'");
	DamageNumber->SetWidgetClass(damageNumberClass); 
	DamageNumber->SetDrawSize(FVector2D(50,5));
	DamageNumber->SetWidgetSpace(EWidgetSpace::Screen); 
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hitted: Hitted(); break;
	case EStateType::Dead:	Dead(); break;
	case EStateType::Dash: Dash(); break;
	}
}

void ACEnemy::BeginPlay()
{
	UMaterialInstanceConstant* body; 
	UMaterialInstanceConstant* logo; 

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/Materials/M_UE4Man_Body_Inst.M_UE4Man_Body_Inst'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/Materials/M_UE4Man_ChestLogo_Inst.M_UE4Man_ChestLogo_Inst'");

	BodyMaterial = UMaterialInstanceDynamic::Create(body, this); 
	LogoMaterial = UMaterialInstanceDynamic::Create(logo, this); 

	GetMesh()->SetMaterial(0, BodyMaterial); 
	GetMesh()->SetMaterial(1, LogoMaterial); 

	Super::BeginPlay();

	HealthWidget->InitWidget(); 
	Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth()); 

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	DamageNumber->InitWidget();
	Cast<UCUserWidget_DamageNumber>(DamageNumber->GetUserWidgetObject())->UpdateDamage(DamageValue);
	DamageNumber->SetVisibility(false);

	Action->SetUnarmedMode(); 
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACEnemy::Hitted()
{
	Status->SubHealth(DamageValue); 
	Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth()); 
	DamageValue = 0.0f; 

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();
		return;
	}

	FVector start = GetActorLocation(); 
	FVector target = DamageInstigator->GetPawn()->GetActorLocation();
	//SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target)); 
	DamageInstigator = NULL; 
	
	FVector direction = target - start; 
	direction.Normalize(); 
	LaunchCharacter(-direction * LaunchAmount, true, true);

	Status->SetStop(); 
	Montages->PlayHitted(); 

	ChangeColor(FLinearColor(1, 0, 0, 1)); 
	UKismetSystemLibrary::K2_SetTimer(this, "RestoreColor", 0.5f, false); 
}

void ACEnemy::Dead()
{
	CheckFalse(State->IsDeadMode()); 

	Montages->PlayDead(); 

	FTimerHandle WaitHandle;
	float WaitTime = 2.0f;

	GetWorld()->GetTimerManager().SetTimer(WaitHandle, this, &ACEnemy::SpawnPotion, WaitTime, false);
}

void ACEnemy::ChangeColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor); 
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor); 
}

void ACEnemy::Begin_Dead()
{
	Action->OffAllCollision();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
}

void ACEnemy::End_Dead()
{
	Destroy();
}

void ACEnemy::RestoreColor()
{
	FLinearColor color = Action->GetCurrent()->GetEquipmentColor();

	ChangeColor(color); 
}


float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (EventInstigator == UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		DamageInstigator = EventInstigator; 
		DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);  

		State->SetHittedMode();

		Cast<UCUserWidget_DamageNumber>(DamageNumber->GetUserWidgetObject())->UpdateDamage(Damage);

		DamageNumber->SetVisibility(true);
	}

	return Status->GetHealth();
}

void ACEnemy::SpawnPotion()
{
	int32 PotionType = FMath::RandRange(0, 2);

	//CLog::Print(PotionType);

	if (PotionType == 1)
	{
		FName path = TEXT("Blueprint'/Game/Action/BPC_PickUpPotion_Health.BPC_PickUpPotion_Health_C'");
		UClass* HealthPotionBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *path.ToString()));
		GetWorld()->SpawnActor<AActor>(HealthPotionBP, GetActorTransform());
	}
	else if (PotionType == 2)
	{
		FName path = TEXT("Blueprint'/Game/Action/BPC_PickUpPotion_Mana.BPC_PickUpPotion_Mana_C'");
		UClass* ManaPotionBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *path.ToString()));
		GetWorld()->SpawnActor<AActor>(ManaPotionBP, GetActorTransform());
	}

	
	/*switch (randomPotion)
	{
	case 0:
		break;
	case 1:
	{
		FName path = TEXT("Blueprint'/Game/Action/BPC_PickUpPotion_Health.BPC_PickUpPotion_Health_C'");
		UClass* HealthPotionBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *path.ToString()));
		GetWorld()->SpawnActor<AActor>(HealthPotionBP, GetActorTransform());
	}
	break;
	case 2:
	{
		FName path = TEXT("Blueprint'/Game/Action/BPC_PickUpPotion_Mana.BPC_PickUpPotion_Mana_C'");
		UClass* ManaPotionBP = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, *path.ToString()));
		GetWorld()->SpawnActor<AActor>(ManaPotionBP, GetActorTransform());
	}
	break;
	default:
		break;
	}*/
}