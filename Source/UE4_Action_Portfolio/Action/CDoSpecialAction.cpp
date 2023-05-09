#include "Action/CDoSpecialAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Component/CStateComponent.h"
#include "Component/CStatusComponent.h"

ACDoSpecialAction::ACDoSpecialAction()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACDoSpecialAction::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner()); 
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter); 
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter); 

	Super::BeginPlay();
}

void ACDoSpecialAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

