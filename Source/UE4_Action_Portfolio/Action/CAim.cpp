#include "Action/CAim.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"

UCAim::UCAim()
{
	CHelpers::GetAssetDynamic<UCurveFloat>(&Curve, "CurveFloat'/Game/Action/C_Aim.C_Aim'");
}

void UCAim::BeginPlay(ACharacter* InCharacter)
{
	OwnerCharacter = InCharacter; 

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter); 

	/*TimelineFloat.BindUFunction(this, "Zooming");

	Timeline.AddInterpFloat(Curve, TimelineFloat);

	Timeline.SetPlayRate(50.0f);
	Timeline.SetTimelineLength(10.0f); 
	Timeline.SetPlaybackPosition(0.0f, true); */
}

void UCAim::OnZoom()
{
	CheckFalse(IsAvaliable());
	CheckTrue(bInZoom);

	bInZoom = true;

	//SpringArm->TargetArmLength = 500.0f; 

	//TargetOffset = FVector(0, 0, 0); 
	//CurrentOffset = SpringArm->SocketOffset; 

	//SpringArm->SocketOffset = NewOffset; 
	//SpringArm->bEnableCameraLag = false;

	//Timeline.PlayFromStart();
}

void UCAim::OffZoom()
{
	CheckFalse(bInZoom); 

	bInZoom = false;

	//TargetOffset = FVector(0, 50, 50);
	//CurrentOffset = SpringArm->SocketOffset;

	////SpringArm->TargetArmLength = 700.0f;
	//SpringArm->SocketOffset = NewOffset; 
	//SpringArm->bEnableCameraLag = true;

	//Timeline.ReverseFromEnd();
}

void UCAim::Tick(float DeltaTime)
{
	//Timeline.TickTimeline(DeltaTime);

	/*if (bInZoom)
		NewOffset = FMath::VInterpTo(CurrentOffset, TargetOffset, DeltaTime, 500);
	else
		NewOffset = FMath::VInterpTo(CurrentOffset, TargetOffset, DeltaTime, 500); */
}

void UCAim::Zooming(float Output)
{
	CLog::Print(Output, 0);

	Camera->FieldOfView = Output;
}