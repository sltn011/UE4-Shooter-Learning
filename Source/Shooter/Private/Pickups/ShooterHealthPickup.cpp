// Shooter, All Rights Reserved


#include "Pickups/ShooterHealthPickup.h"

#include "Components/ShooterHealthComponent.h"
#include "Player/ShooterBaseCharacter.h"
#include "ShooterUtils.h"

void AShooterHealthPickup::BeginPlay(
)
{
    Super::BeginPlay();

    check(HealthRestoredAmount >= 0.0f);
}

bool AShooterHealthPickup::IsPickableCondition(
    AActor *PickerActor
)
{
    bool ParentValue = Super::IsPickableCondition(PickerActor);

    UShooterHealthComponent *HealthComponent = ShooterUtils::GetPlayerComponentByClass<UShooterHealthComponent>(PickerActor);
    if (!HealthComponent || HealthComponent->IsDead()) {
        return false;
    }

    if (FMath::IsNearlyEqual(HealthComponent->GetHealthPercent(), 1.0f)) {
        return false;
    }

    return ParentValue;
}

void AShooterHealthPickup::PickupEffect(
    APawn *PickerPawn
)
{
    AShooterBaseCharacter *HealedCharacter = Cast<AShooterBaseCharacter>(PickerPawn);
    if (!HealedCharacter) {
        return;
    }

    UShooterHealthComponent *HealthComponent = Cast<UShooterHealthComponent>(HealedCharacter->GetComponentByClass(UShooterHealthComponent::StaticClass()));
    if (!HealthComponent) {
        return;
    }

    HealthComponent->RestoreHealth(HealthRestoredAmount);

}
