#include "CStatusComponent.h"
#include "Global.h"

UCStatusComponent::UCStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();


}

void UCStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UCStatusComponent::AddHealth(float InAmount)
{
	Health += InAmount; 
	Health = FMath::Clamp(Health, 0.0f, MaxHealth); 
}

void UCStatusComponent::SubHealth(float InAmount)
{
	Health -= InAmount; 
	Health = FMath::Clamp(Health, 0.0f, MaxHealth); 
}

void UCStatusComponent::AddMana(float InAmount)
{
	Mana += InAmount; 
	Mana = FMath::Clamp(Mana, 0.0f, MaxMana); 
}

void UCStatusComponent::SubMana(float InAmount)
{
	Mana -= InAmount; 
	Mana = FMath::Clamp(Mana, 0.0f, MaxMana); 
}

void UCStatusComponent::SetMove()
{
	bCanMove = true; 
}

void UCStatusComponent::SetStop()
{
	bCanMove = false; 
}


