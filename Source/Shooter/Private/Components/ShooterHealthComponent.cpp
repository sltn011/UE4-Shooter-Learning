// Shooter, All Rights Reserved


#include "Components/ShooterHealthComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterHealthComponent, All, All);

UShooterHealthComponent::UShooterHealthComponent(
)
{
	PrimaryComponentTick.bCanEverTick = false;
}


float UShooterHealthComponent::RestoreHealth(
	float HealthRestoredAmount
)
{
	if (HealthRestoredAmount <= 0.0f) {
		return 0.0f;
	}

	float HealthBefore = Health;
	SetHealth(Health + HealthRestoredAmount);

	UE_LOG(LogShooterHealthComponent, Display, TEXT("Health Restore Log: Character: %s, Before: %.3f, After: %.3f, Restored: %.3f"),
		*GetOwner()->GetName(), HealthBefore, Health, Health - HealthBefore
	);

	return Health - HealthBefore;
}

float UShooterHealthComponent::GetHealth(
) const
{
	return Health;
}

float UShooterHealthComponent::GetHealthPercent(
) const
{
	return Health / MaxHealth;
}

bool UShooterHealthComponent::IsDead(
) const
{
	return FMath::IsNearlyZero(Health);
}

void UShooterHealthComponent::BeginPlay(
)
{
	Super::BeginPlay();

	check(MaxHealth > 0.0f);
	check(AutoHealStartDelay >= 0.0f);
	check(AutoHealUpdateTime >= 0.0f);
	check(AutoHealAddedValue >= 0.0f);

	SetHealth(MaxHealth);
	
	AActor *Owner = GetOwner();
	if (Owner) {
		Owner->OnTakeAnyDamage.AddDynamic(this, &UShooterHealthComponent::OnTakeAnyDamage);
	}
	else {
		UE_LOG(LogShooterHealthComponent, Error, TEXT("Error recieving component's Owner!"));
		checkNoEntry();
	}
}

void UShooterHealthComponent::OnTakeAnyDamage(
	AActor *DamagedActor, 
	float Damage, 
	UDamageType const *DamageType, 
	AController *InstigatedBy,
	AActor *DamageCauser
)
{
	if (Damage <= 0 || IsDead() || !GetWorld()) {
		return;
	}

	SetHealth(Health - Damage);

	GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);

	if (IsDead()) {
		OnDeath.Broadcast();
	}
	else if (bAutoHeal) {
		if (GetWorld()) {
			GetWorld()->GetTimerManager().SetTimer(AutoHealTimerHandle, this, &UShooterHealthComponent::AutoHeal, AutoHealUpdateTime, true, AutoHealStartDelay);
		}
	}
}

void UShooterHealthComponent::SetHealth(
	float NewHealth
)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}

void UShooterHealthComponent::AutoHeal(
)
{
	SetHealth(Health + AutoHealAddedValue);

	if (FMath::IsNearlyEqual(Health, MaxHealth)) {
		if (GetWorld()) {
			GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
		}
	}
}
