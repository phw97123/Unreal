#include "Character/CEnemy_Dash.h"
#include "Global.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "CAIController_Enemy.h"
#include "Component/CActionComponent.h"

ACEnemy_Dash::ACEnemy_Dash()
{
	AIControllerClass = ACAIController_Enemy::StaticClass(); 
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; 

	USkeletalMesh* mesh;
	CHelpers::GetAsset(&mesh, "SkeletalMesh'/Game/UndeadPack/Ghoul/Mesh/SK_Ghoul_Full.SK_Ghoul_Full'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass(&animInstance, "AnimBlueprint'/Game/Enemy/Enemy_Dash/ABPC_Enemy_Dash.ABPC_Enemy_Dash_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);
}

void ACEnemy_Dash::BeginPlay()
{
	Super::BeginPlay(); 

	UMaterialInstanceConstant* body;
	UMaterialInstanceConstant* logo;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/UndeadPack/Ghoul/Materials/MI_Ghoul_Decor.MI_Ghoul_Decor'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/UndeadPack/Ghoul/Materials/MI_Ghoul.MI_Ghoul'");

	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(logo, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	Action->SetSwordMode();
}

void ACEnemy_Dash::Attack()
{
	CheckFalse(State->IsIdleMode());

	Action->DoAction();

	const FVector ForwardDir = this->GetActorRotation().Vector();
	LaunchCharacter(ForwardDir * 3000.0f, true, true);
}

void ACEnemy_Dash::EndAttack()
{
	OnAttackEnd.Broadcast(); 
}

