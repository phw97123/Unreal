#include "CMontageComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCMontageComponent::UCMontageComponent()
{
}

void UCMontageComponent::BeginPlay()
{
	Super::BeginPlay();

	CheckNull(DataTable); 

	TArray<FMontageData*> datas; 
	DataTable->GetAllRows<FMontageData>("", datas);

	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		for (FMontageData* data : datas)
		{
			if ((EStateType)i == data->Type)
			{
				Datas[i] = data;

				continue;
			}
		}
	}
}

void UCMontageComponent::PlayDash()
{
	PlayAnimMontage(EStateType::Dash); 

	//CLog::Print("Dash");
}

void UCMontageComponent::PlayHitted()
{
	PlayAnimMontage(EStateType::Hitted); 
}

void UCMontageComponent::PlayDead()
{
	PlayAnimMontage(EStateType::Dead); 
}

void UCMontageComponent::PlayAnimMontage(EStateType InState)
{
	ACharacter* character = Cast<ACharacter>(GetOwner()); 

	const FMontageData* data = Datas[(int32)InState]; 
	if (!!data)
	{
		if (!!data->AnimMontage)
		{
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
		}
	}
}
