#include "Character/CEnemy_Wizard.h"
#include "Global.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "CAIController_Enemy.h"
#include "Component/CActionComponent.h"

ACEnemy_Wizard::ACEnemy_Wizard()
{
	AIControllerClass = ACAIController_Enemy::StaticClass(); 
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; 

	USkeletalMesh* mesh;
	CHelpers::GetAsset(&mesh, "SkeletalMesh'/Game/UndeadPack/SkeletonEnemy/Mesh/SK_Skeleton.SK_Skeleton'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass(&animInstance, "AnimBlueprint'/Game/Enemy/Enemy_Wizard/ABPC_Enemy_Wizard.ABPC_Enemy_Wizard_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);
}

void ACEnemy_Wizard::BeginPlay()
{
	Super::BeginPlay(); 

	UMaterialInstanceConstant* body;
	UMaterialInstanceConstant* logo;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/UndeadPack/SkeletonEnemy/Materials/MI_Skeleton_02.MI_Skeleton_02'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/UndeadPack/SkeletonEnemy/Materials/MI_Skeleton_02.MI_Skeleton_02'");

	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(logo, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	Action->SetSwordMode(); 
}

void ACEnemy_Wizard::Attack()
{
	Action->DoAction(); 
}

void ACEnemy_Wizard::EndAttack()
{
	OnAttackEnd.Broadcast(); 
}