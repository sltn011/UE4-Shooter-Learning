// Shooter, All Rights Reserved


#include "Components/ShooterHealthComponent.h"

#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UShooterHealthComponent::UShooterHealthComponent(
)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


float UShooterHealthComponent::GetHealth(
) const
{
	return Health;
}

bool UShooterHealthComponent::IsDead(
) const
{
	return FMath::IsNearlyZero(Health);
}

// Called when the game starts
void UShooterHealthComponent::BeginPlay(
)
{
	Super::BeginPlay();

	SetHealth(MaxHealth);
	
	AActor *Owner = GetOwner();
	if (Owner) {
		Owner->OnTakeAnyDamage.AddDynamic(this, &UShooterHealthComponent::OnTakeAnyDamage);
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
