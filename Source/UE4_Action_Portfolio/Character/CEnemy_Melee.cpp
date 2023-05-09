#include "CEnemy_Melee.h"
#include "Global.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "CAIController_Enemy.h"
#include "Component/CActionComponent.h"

ACEnemy_Melee::ACEnemy_Melee()
{
	AIControllerClass = ACAIController_Enemy::StaticClass(); 
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	USkeletalMesh* mesh;
	CHelpers::GetAsset(&mesh, "SkeletalMesh'/Game/UndeadPack/SkeletonEnemy/Enemy_Mesh/Mesh/SK_Skeleton.SK_Skeleton'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass(&animInstance, "AnimBlueprint'/Game/Enemy/Enemy_Melee/ABPC_MeleeEnemy.ABPC_MeleeEnemy_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);
}

void ACEnemy_Melee::BeginPlay()
{
	Super::BeginPlay(); 

	UMaterialInstanceConstant* body;
	UMaterialInstanceConstant* logo;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/UndeadPack/SkeletonEnemy/Enemy_Mesh/Materials/MI_Skeleton_01.MI_Skeleton_01'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/UndeadPack/SkeletonEnemy/Enemy_Mesh/Materials/MI_Skeleton_01.MI_Skeleton_01'");

	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(logo, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	Action->SetSwordMode();
}

void ACEnemy_Melee::Attack()
{
	Action->DoAction(); 
}

void ACEnemy_Melee::EndAttack()
{
	OnAttackEnd.Broadcast();
}
