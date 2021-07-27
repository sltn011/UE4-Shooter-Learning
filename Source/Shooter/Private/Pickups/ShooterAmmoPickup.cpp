// Shooter, All Rights Reserved


#include "Pickups/ShooterAmmoPickup.h"

#include "Components/ShooterWeaponComponent.h"
#include "Player/ShooterBaseCharacter.h"
#include "Weapon/ShooterBaseWeapon.h"

void AShooterAmmoPickup::BeginPlay(
)
{
    Super::BeginPlay();

    check(RestockedWeaponClass);
}

void AShooterAmmoPickup::PickupEffect(
    APawn *PickerPawn
)
{
    AShooterBaseCharacter *PickerCharacter = Cast<AShooterBaseCharacter>(PickerPawn);
    if (!PickerCharacter) {
        return;
    }

    UShooterWeaponComponent *PickerWeaponComponent = Cast<UShooterWeaponComponent>(PickerCharacter->GetComponentByClass(UShooterWeaponComponent::StaticClass()));
    if (!PickerWeaponComponent) {
        return;
    }

    PickerWeaponComponent->AddAmmoToWeapon(AmmoRestoreType, RestockedWeaponClass, ClipsRestored, BulletsRestored);
}

#if WITH_EDITOR
bool AShooterAmmoPickup::CanEditChange(
    FProperty const *InProperty
) const
{
    const bool ParentValue = Super::CanEditChange(InProperty);

    if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(AShooterAmmoPickup, ClipsRestored)) {
        return ParentValue && (AmmoRestoreType == EAmmoRestoreType::ClipsOnly || AmmoRestoreType == EAmmoRestoreType::ClipsAndBullets);
    }

    if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(AShooterAmmoPickup, BulletsRestored)) {
        return ParentValue && (AmmoRestoreType == EAmmoRestoreType::BulletsOnly || AmmoRestoreType == EAmmoRestoreType::ClipsAndBullets);
    }

    return ParentValue;
}
#endif
