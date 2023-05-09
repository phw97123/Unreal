#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ChildActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Component/COptionComponent.h"
#include "Component/CStateComponent.h"
#include "Component/CStatusComponent.h"
#include "Component/CMontageComponent.h"
#include "Component/CActionComponent.h"
#include "Component/UCTargetComponent.h"
#include "Component/CFeetComponent.h"
#include "Animation/AnimInstance.h"
#include "Action/CAttachment.h"
#include "Action/CPickUp.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Widget/CUserWidget_CrossHair.h"
#include "Widget/CUserWidget_Health.h"
#include "Widget/CUserWidget_SkillSlot.h"
#include "Widget/CUserWidget_NumberOfEnemies.h"
#include "Widget/CUserWidget_ActionList.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm); 
	CHelpers::CreateComponent<UChildActorComponent>(this, &MainCamChild, "MainCamChild", Camera);

	CHelpers::CreateComponent<UCameraComponent>(this, &SubCamera1, "SubCamera1", SpringArm);
	CHelpers::CreateComponent<UChildActorComponent>(this, &SubCamChild1, "SubCamChild1", SubCamera1);

	CHelpers::CreateComponent<UCameraComponent>(this, &SubCamera2, "SubCamera2", SpringArm);
	CHelpers::CreateComponent<UChildActorComponent>(this, &SubCamChild2, "SubCamChild2", SubCamera2);

	CHelpers::CreateComponent<UCameraComponent>(this, &SubCamera3, "SubCamera3", SpringArm);
	CHelpers::CreateComponent<UChildActorComponent>(this, &SubCamChild3, "SubCamChild3", SubCamera3);

	CHelpers::CreateComponent<UCameraComponent>(this, &SubCamera4, "SubCamera4", SpringArm);
	CHelpers::CreateComponent<UChildActorComponent>(this, &SubCamChild4, "SubCamChild4", SubCamera4);

	CHelpers::CreateComponent<UCameraComponent>(this, &SubCamera5, "SubCamera5", SpringArm);
	CHelpers::CreateComponent<UChildActorComponent>(this, &SubCamChild5, "SubCamChild5", SubCamera5);


	CHelpers::CreateActorComponent<UCOptionComponent>(this, &Option, "Option");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCMontageComponent>(this, &Montage, "Montage");
	CHelpers::CreateActorComponent<UCActionComponent>(this, &Action, "Action");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status"); 
	CHelpers::CreateActorComponent<UUCTargetComponent>(this, &Target, "Target");
	CHelpers::CreateActorComponent<UCFeetComponent>(this, &Feet, "Feet"); 

	bUseControllerRotationYaw = false;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh; 
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animinstance;
	CHelpers::GetClass<UAnimInstance>(&animinstance, "AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animinstance);

	SpringArm->SetRelativeLocation(FVector(0, 0, 140)); 
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 250.0f; 
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	SpringArm->SocketOffset = FVector(0, 50, 50); 

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bPickUp = false;
	bItemOverlap = false;

	SpringArm->TargetArmLength = 700.0f;

	Action->SetUnarmedMode();
}

void ACPlayer::GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection)
{
	OutDirection = Camera->GetForwardVector(); 

	FTransform transform = Camera->GetComponentToWorld(); 
	FVector cameraLocation = transform.GetLocation(); 
	OutStart = cameraLocation + OutDirection; 

	FVector conDirection = UKismetMathLibrary::RandomUnitVectorInEllipticalConeInDegrees(OutDirection, 0.2f, 0.3f);

	conDirection *= 1.0f; 

	OutEnd = cameraLocation + conDirection; 
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	ActionBar = CreateWidget<UCUserWidget_Health, APlayerController>(GetController<APlayerController>(), ActionBarClass);
	ActionBar->AddToViewport(); 
	
	CrossHair = CreateWidget<UCUserWidget_CrossHair, APlayerController>(GetController<APlayerController>(), CrossHairClass);
	CrossHair->AddToViewport(); 
	CrossHair->SetVisibility(ESlateVisibility::Hidden); 

	SkillSlot = CreateWidget<UCUserWidget_SkillSlot, APlayerController>(GetController<APlayerController>(), SkillSlotClass);
	SkillSlot->AddToViewport();
	SkillSlot->SetVisibility(ESlateVisibility::Hidden);

	DashSkillSlot = CreateWidget<UCUserWidget_SkillSlot, APlayerController>(GetController<APlayerController>(), DashSkillSlotClass);
	DashSkillSlot->AddToViewport(); 

	ActionList = CreateWidget<UCUserWidget_ActionList, APlayerController>(GetController<APlayerController>(), ActionListClass);
	ActionList->AddToViewport(); 
	ActionList->SetVisibility(ESlateVisibility::Hidden); 

	ActionList->GetData(0).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnSword); 
	ActionList->GetData(1).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnBow);
}

void ACPlayer::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);

	ActionBar->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	ActionBar->UpdateMana(Status->GetMana(), Status->GetMaxMana());

	if (DashSkillCoolDown < 5.0f)
	{
		DashSkillCoolDown += DeltaTime;

		if (DashSkillCoolDown >= 5.0f)
			DashSkillCoolDown = 5.0f;
	}

	if (SkillCoolDown < 8.0f)
	{
		SkillCoolDown += DeltaTime; 

		if (SkillCoolDown >= 8.0f)
			SkillCoolDown = 8.0f;
	}

	if (bIsDashing)
	{
		FVector Velocity = GetVelocity();

		//캐릭터가 지면에 있을 경우 이동 방향에 대시 속도를 곱해줌
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			Velocity += GetActorRotation().Vector() * DashSpeed;
		}
		//캐릭터가 지면에 있지 않은 경우 속도에 대시속도를 추가하고 기울기를 고려하여 Z속도를 조정
		else
		{
			FVector ForwardVector = GetPendingMovementInputVector();
			FVector RightVector = FVector::CrossProduct(ForwardVector, FVector::UpVector);
			FVector UpVector = FVector::CrossProduct(RightVector, ForwardVector).GetSafeNormal();

			FVector SlopeNormal = GetCapsuleComponent()->GetUpVector();
			float DotProduct = FVector::DotProduct(UpVector, SlopeNormal);

			//속도에 대시 속도를 추가하고 기울기를 고려해서 속도의 Z 구성요소를 조정하여 기울기를 설명하고 
			//UpVector와 기울기 법선 사이의 내적으로 크기를 조정
			Velocity += ForwardVector * DashSpeed + UpVector * (DotProduct * DashSpeed);
		}

		GetCharacterMovement()->Velocity = Velocity;

		DashDuration -= DeltaTime;
		if (DashDuration <= 0.f)
		{
			bIsDashing = false;
		}


		/* 
		 * 플레이어가 지형의 경사면을 따라 이동할 때 기울기에 따라 캐릭터의 이동방향과 속도가 변경되므로
		 * 이동 입력 벡터와 수직이면서 지면과 수평인 벡터인 RightVector를 구하고 RightVector와 ForwardVector의 외적을 구해 
		 * UpVector를 게산하게 되면 RightVector와 ForwardVector는 모두 수직이면서 서로 수평인 벡터가 되는데 이는 캐릭터가 
		 * 현재 위치한 경사면의 방향을 나타내는 벡터가 되고 이 UpVector와 캐릭터의 UpVector(위쪽 방향) 간의 내적을 계산하여 
		 * 경사면의 기울기를 구하고 이를 기반으로 대쉬하는 방향에서 적절한 Z축 성분을 추가해줌으로써 캐릭터가 경사면을 따라
		 * 자연스럽게 대쉬할 수 있도록 한다
		*/
	}
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward); 
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);

	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);
	PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::OnZoom);

	PlayerInputComponent->BindAction("Dash",EInputEvent::IE_Pressed, this, &ACPlayer::OnDash);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAction);
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Released, this, &ACPlayer::OffAim);

	PlayerInputComponent->BindAction("Targeting", EInputEvent::IE_Released, this, &ACPlayer::OnTarget);

	PlayerInputComponent->BindAction("SpecialAttack", EInputEvent::IE_Pressed, this, &ACPlayer::OnSpecialAction); 

	PlayerInputComponent->BindAction("ViewActionList", EInputEvent::IE_Pressed, this, &ACPlayer::OnViewActionList); 
	PlayerInputComponent->BindAction("ViewActionList", EInputEvent::IE_Released, this, &ACPlayer::OffViewActionList); 
}

void ACPlayer::OnMoveForward(float InAxis)
{
	CheckFalse(Status->CanMove()); 

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0); 
	FVector direction = FQuat(rotator).GetForwardVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{
	CheckFalse(Status->CanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	float rate = Option->GetHorizontalLookRate();
	AddControllerYawInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	float rate = Option->GetVerticalLookRate();
	AddControllerPitchInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnZoom(float InAxis)
{
	SpringArm->TargetArmLength += (1000.0f * InAxis * GetWorld()->GetDeltaSeconds());
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, 50.0f, 700.0f);
}

void ACPlayer::OnDash()
{
	if (DashSkillCoolDown >= 5.0)
	{
		State->SetDashMode();
		DashSkillCoolDown = 0.0f; 
	}	
}

void ACPlayer::OnTarget()
{
	Target->ToggleTarget(); 
}

void ACPlayer::OnDoAction()
{
	if (Action->IsBowMode())
		OnAim();
	else
		Action->DoAction(); 
}

void ACPlayer::OnSpecialAction()
{
	if (SkillCoolDown >= 8.0f && Status->GetMana()>= 30.0f)
	{
		Action->DoSpecialAction();
		SkillCoolDown = 0.0f;
		Status->SubMana(30.0f);
	}
}

void ACPlayer::OnAim()
{
	bUseControllerRotationYaw = true; 
	GetCharacterMovement()->bOrientRotationToMovement = false;

	CrossHair->SetVisibility(ESlateVisibility::Visible); 

	Action->DoAim();
}

void ACPlayer::OffAim()
{
	if (Action->IsBowMode())
	{
		Action->DoAction(); 

		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		
		CrossHair->SetVisibility(ESlateVisibility::Hidden); 

		Action->UnDoAim();
	}
}

void ACPlayer::OnViewActionList()
{
	CheckFalse(State->IsIdleMode()); 
	ActionList->SetVisibility(ESlateVisibility::Visible); 

	GetController<APlayerController>()->bShowMouseCursor = true; 
	GetController<APlayerController>()->SetInputMode(FInputModeGameAndUI()); 

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f); 
}

void ACPlayer::OffViewActionList()
{
	ActionList->SetVisibility(ESlateVisibility::Hidden);
	
	GetController<APlayerController>()->bShowMouseCursor = false; 
	GetController<APlayerController>()->SetInputMode(FInputModeGameOnly()); 

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f); 
}

void ACPlayer::OnStateTypeChanged(EStateType InprevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Dash: Begin_Dash(); break;
	case EStateType::Hitted: Hitted(); break; 
	case EStateType::Dead: Dead(); break;
	}
}

void ACPlayer::Begin_Dash()
{
	if (CanDash())
	{
		Montage->PlayDash(); 

		bIsDashing = true;  
		DashDuration = 0.2f;

		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;

		FVector start = GetActorLocation();
		FVector from = start + GetVelocity().GetSafeNormal2D();
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, from));
	}
}

void ACPlayer::Hitted()
{
	Status->SubHealth(DamageValue); 
	ActionBar->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	DamageValue = 0.0f; 

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode(); 
		return; 
	}

	Status->SetStop(); 
	Montage->PlayHitted(); 
}

void ACPlayer::Dead()
{
	CheckFalse(State->IsDeadMode()); 

	Montage->PlayDead(); 
}


bool ACPlayer::CanDash() const
{
	return !bIsDashing && GetCharacterMovement()->IsMovingOnGround(); 
}

void ACPlayer::End_Dash()
{
	if (Action->IsUnarmedMode() == false)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	bIsDashing = false; 

	State->SetIdleMode();
}

void ACPlayer::ChangeColor(FLinearColor InColor)
{
}

void ACPlayer::Begin_Dead()
{
	Action->OffAllCollision();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACPlayer::End_Dead()
{
	Destroy();

	//UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(), false);
}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//ACharacter* character = Cast<ACharacter>(DamageCauser); 

	DamageInstigator = EventInstigator;
	DamageValue = Super::TakeDamage(Damage, DamageEvent, DamageInstigator, DamageCauser);

	State->SetHittedMode();

	return Status->GetHealth(); 
}

void ACPlayer::SetPickUp(bool val)
{
	bPickUp = val;
}

void ACPlayer::SetbItemOverlap(bool val)
{
	bItemOverlap = val;
}

void ACPlayer::OnSword()
{
	//CheckFalse(State->IsIdleMode()); 
	SkillSlot->SetVisibility(ESlateVisibility::Visible);
	SkillCoolDown = 8.0f;
	Action->SetSwordMode();
	SkillSlot->SwordSkillSlot();
}

void ACPlayer::OnBow()
{
	//CheckFalse(State->IsIdleMode()); 
	SkillSlot->SetVisibility(ESlateVisibility::Visible);
	SkillCoolDown = 8.0f;
	Action->SetBowMode(); 
	SkillSlot->BowSkillSlot();
}

void ACPlayer::PickUpHealthPotion(float InAmount)
{
	Status->AddHealth(InAmount);
}

void ACPlayer::PickUpManaPotion(float InAmount)
{
	Status->AddMana(InAmount); 
}

void ACPlayer::OnSpringArmRotation()
{
	SpringArm->bUsePawnControlRotation = false;
}

void ACPlayer::OffSpringArmRotation()
{
	SpringArm->bUsePawnControlRotation = true;
}

void ACPlayer::SpringArmChange(FRotator rotator, float TargetArmLength)
{
	SpringArm->SetRelativeRotation(rotator);
	SpringArm->TargetArmLength = TargetArmLength;
}

void ACPlayer::VisibillityNumberOfEnemies()
{
	numberofEnemies = CreateWidget<UCUserWidget_NumberOfEnemies, APlayerController>(GetController<APlayerController>(), numberofEnemiesClass);
	numberofEnemies->AddToViewport();
	numberofEnemies->SetVisibility(ESlateVisibility::Visible);
}

void ACPlayer::EquipSkillSlot()
{
	SkillSlot->SetVisibility(ESlateVisibility::Hidden);
}

