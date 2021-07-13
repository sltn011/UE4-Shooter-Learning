// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UShooterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShooterHealthComponent();

	UFUNCTION(BlueprintCallable)
	float GetHealth(
	) const;

	UFUNCTION(BlueprintCallable)
	bool IsDead(
	) const;

	FOnDeath OnDeath;

	FOnHealthChanged OnHealthChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay(
	) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = 0.0f))
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Healing")
	bool bAutoHeal = true;

	UPROPERTY(EditDefaultsOnly, Category = "Healing", meta = (ClampMin = 0.0f, EditCondition = "bAutoHeal"))
	float AutoHealStartDelay = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Healing", meta = (ClampMin = 0.0f, EditCondition = "bAutoHeal"))
	float AutoHealUpdateTime = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Healing", meta = (ClampMin = 0.0f, EditCondition = "bAutoHeal"))
	float AutoHealAddedValue = 1.0f;

private:

	float Health = 0.0f;

	FTimerHandle AutoHealTimerHandle;

	UFUNCTION()
	void OnTakeAnyDamage(
		AActor *DamagedActor,
		float Damage,
		UDamageType const *DamageType,
		AController *InstigatedBy,
		AActor *DamageCauser
	);

	void SetHealth(float NewHealth);

	void AutoHeal();
};
