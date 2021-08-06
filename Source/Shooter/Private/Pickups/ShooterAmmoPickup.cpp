// Shooter, All Rights Reserved


#include "Pickups/ShooterAmmoPickup.h"

#include "Components/ShooterWeaponComponent.h"
#include "Player/ShooterBaseCharacter.h"
#include "ShooterUtils.h"
#include "Weapon/ShooterBaseWeapon.h"

void AShooterAmmoPickup::BeginPlay(
)
{
    Super::BeginPlay();

    check(RestockedWeaponClass);
}

bool AShooterAmmoPickup::IsPickableCondition(
    AActor *PickerActor
)
{
    bool ParentValue = Super::IsPickableCondition(PickerActor);

    UShooterWeaponComponent *WeaponComponent = ShooterUtils::GetPlayerComponentByClass<UShooterWeaponComponent>(PickerActor);
    if (!WeaponComponent) {
        return false;
    }

    FAmmoData DefaultAmmo, CurrentAmmo;
    if (!WeaponComponent->GetWeaponFullAmmoDataByClass(RestockedWeaponClass, DefaultAmmo, CurrentAmmo)) {
        return false;
    }

    if (CurrentAmmo.bInfiniteAmmo) {
        return false;
    }

    int32 DefaultBullets = DefaultAmmo.BulletsInClip * DefaultAmmo.Clips + DefaultAmmo.BulletsInClip;
    int32 CurrentBullets = CurrentAmmo.SpareAmmo + CurrentAmmo.BulletsInClip;

    float AmmoPercent = static_cast<float>(CurrentBullets) / static_cast<float>(DefaultBullets);

    if (FMath::IsNearlyEqual(AmmoPercent, 1.0f)) {
        return false;
    }

    return ParentValue;
}

void AShooterAmmoPickup::PickupEffect(
    APawn *PickerPawn
)
{
    AShooterBaseCharacter *PickerCharacter = Cast<AShooterBaseCharacter>(PickerPawn);
    if (!PickerCharacter) {
        return;
    }

    UShooterWeaponComponent *WeaponComponent = ShooterUtils::GetPlayerComponentByClass<UShooterWeaponComponent>(PickerCharacter);
    if (!WeaponComponent) {
        return;
    }

    WeaponComponent->AddAmmoToWeapon(AmmoRestoreType, RestockedWeaponClass, ClipsRestored, BulletsRestored);
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
