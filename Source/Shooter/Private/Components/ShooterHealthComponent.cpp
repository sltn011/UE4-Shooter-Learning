// Shooter, All Rights Reserved


#include "Components/ShooterHealthComponent.h"

#include "GameFramework/Actor.h"

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
	return Health <= 0.0f;
}

// Called when the game starts
void UShooterHealthComponent::BeginPlay(
)
{
	Super::BeginPlay();

	Health = MaxHealth;
	OnHealthChanged.Broadcast(Health);
	
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
	if (Damage <= 0 || IsDead()) {
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
	
	if (IsDead()) {
		OnDeath.Broadcast();
	}
}

