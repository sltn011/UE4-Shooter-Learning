// Shooter, All Rights Reserved


#include "Pickups/ShooterHealthPickup.h"

#include "Components/ShooterHealthComponent.h"
#include "Player/ShooterBaseCharacter.h"

void AShooterHealthPickup::BeginPlay(
)
{
    Super::BeginPlay();

    check(HealthRestoredAmount >= 0.0f);
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
