// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterCoreTypes.h"
#include "ShooterHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UShooterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

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

	float Health = 0.0f;

	FTimerHandle AutoHealTimerHandle;
};
