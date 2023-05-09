             #pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4_ACTION_PORTFOLIO_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

private: 
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth = 100; 

	UPROPERTY(EditDefaultsOnly, Category = "Mana")
		float MaxMana = 100; 

	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float Health;

	UPROPERTY(EditDefaultsOnly, Category = "Mana")
		float Mana;

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
		float WalkSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
		float RunSpeed = 600.0f; 

public: 
	FORCEINLINE	float GetMaxHealth() { return MaxHealth;  }
	FORCEINLINE	float GetHealth() { return Health; }
	FORCEINLINE float GetMaxMana() { return MaxMana;  }	
	FORCEINLINE	float GetMana() { return Mana;  }
	FORCEINLINE float GetWalkSpeed() { return WalkSpeed;  }	
	FORCEINLINE float GetRunSpeed() { return RunSpeed;  }	

	FORCEINLINE bool CanMove() { return bCanMove; }	

public:	
	UCStatusComponent();

	void AddHealth(float InAmount); 
	void SubHealth(float InAmount); 

	void AddMana(float InAmount); 
	void SubMana(float InAmount); 

	void SetMove(); 
	void SetStop(); 

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private: 
	bool bCanMove = true; 
		
};
