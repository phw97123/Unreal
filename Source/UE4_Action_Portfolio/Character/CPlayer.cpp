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

		//ĳ���Ͱ� ���鿡 ���� ��� �̵� ���⿡ ��� �ӵ��� ������
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			Velocity += GetActorRotation().Vector() * DashSpeed;
		}
		//ĳ���Ͱ� ���鿡 ���� ���� ��� �ӵ��� ��üӵ��� �߰��ϰ� ���⸦ ����Ͽ� Z�ӵ��� ����
		else
		{
			FVector ForwardVector = GetPendingMovementInputVector();
			FVector RightVector = FVector::CrossProduct(ForwardVector, FVector::UpVector);
			FVector UpVector = FVector::CrossProduct(RightVector, ForwardVector).GetSafeNormal();

			FVector SlopeNormal = GetCapsuleComponent()->GetUpVector();
			float DotProduct = FVector::DotProduct(UpVector, SlopeNormal);

			//�ӵ��� ��� �ӵ��� �߰��ϰ� ���⸦ ����ؼ� �ӵ��� Z ������Ҹ� �����Ͽ� ���⸦ �����ϰ� 
			//UpVector�� ���� ���� ������ �������� ũ�⸦ ����
			Velocity += ForwardVector * DashSpeed + UpVector * (DotProduct * DashSpeed);
		}

		GetCharacterMovement()->Velocity = Velocity;

		DashDuration -= DeltaTime;
		if (DashDuration <= 0.f)
		{
			bIsDashing = false;
		}


		/* 
		 * �÷��̾ ������ ������ ���� �̵��� �� ���⿡ ���� ĳ������ �̵������ �ӵ��� ����ǹǷ�
		 * �̵� �Է� ���Ϳ� �����̸鼭 ����� ������ ������ RightVector�� ���ϰ� RightVector�� ForwardVector�� ������ ���� 
		 * UpVector�� �Ի��ϰ� �Ǹ� RightVector�� ForwardVector�� ��� �����̸鼭 ���� ������ ���Ͱ� �Ǵµ� �̴� ĳ���Ͱ� 
		 * ���� ��ġ�� ������ ������ ��Ÿ���� ���Ͱ� �ǰ� �� UpVector�� ĳ������ UpVector(���� ����) ���� ������ ����Ͽ� 
		 * ������ ���⸦ ���ϰ� �̸� ������� �뽬�ϴ� ���⿡�� ������ Z�� ������ �߰��������ν� ĳ���Ͱ� ������ ����
		 * �ڿ������� �뽬�� �� �ֵ��� �Ѵ�
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

