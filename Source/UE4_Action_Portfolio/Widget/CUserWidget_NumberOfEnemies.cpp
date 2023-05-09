#include "Widget/CUserWidget_NumberOfEnemies.h"
#include "Global.h"
#include "Character/CEnemy.h"
#include "Interaction/CBossStagePotal.h"

int32 UCUserWidget_NumberOfEnemies::CountAliveEnemies()
{
	TArray<AActor*> enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACEnemy::StaticClass(), enemies);

	int32 AliveEnemies = 0;
	for (AActor* EnemyActor : enemies)
	{
		ACEnemy* enemy = Cast<ACEnemy>(EnemyActor);
		if (enemy && !enemy->State->IsDeadMode())
			AliveEnemies++;
	}

	return AliveEnemies - 1;
}

void UCUserWidget_NumberOfEnemies::SpawnPotal()
{
	if (BossStagePotalClass && CountAliveEnemies() <=0)
	{
		ACBossStagePotal* PotalActor = GetWorld()->SpawnActor<ACBossStagePotal>(BossStagePotalClass, FVector(2169.0f, 200.0f, 8510.0f), FRotator(0, 0, 0));
	}
}
