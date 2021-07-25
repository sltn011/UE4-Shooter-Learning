// Shooter, All Rights Reserved


#include "UI/ShooterPlayerHUDWidget.h"

#include "Components/ShooterHealthComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "ShooterUtils.h"

float UShooterPlayerHUDWidget::GetHealthPercent(
) const
{
    UShooterHealthComponent const *PlayerHealthComponent = ShooterUtils::GetPlayerComponentByClass<UShooterHealthComponent>(GetOwningPlayerPawn());

    if (!PlayerHealthComponent) {
        return 0.0f;
    }

    return PlayerHealthComponent->GetHealthPercent();
}

bool UShooterPlayerHUDWidget::IsPlayerAlive(
) const
{
    UShooterHealthComponent const *PlayerHealthComponent = ShooterUtils::GetPlayerComponentByClass<UShooterHealthComponent>(GetOwningPlayerPawn());
    return PlayerHealthComponent && !PlayerHealthComponent->IsDead();
}

bool UShooterPlayerHUDWidget::IsPlayerSpectating(
) const
{
    AController const *Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

bool UShooterPlayerHUDWidget::GetCurrentWeaponUIData(
    FWeaponUIData &UIData
) const
{
    UShooterWeaponComponent const *PlayerWeaponComponent = ShooterUtils::GetPlayerComponentByClass<UShooterWeaponComponent>(GetOwningPlayerPawn());

    if (!PlayerWeaponComponent) {
        return false;
    }

    return PlayerWeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool UShooterPlayerHUDWidget::GetCurrentWeaponAmmoData(
    FAmmoData &AmmoData
) const
{
    UShooterWeaponComponent const *PlayerWeaponComponent = ShooterUtils::GetPlayerComponentByClass<UShooterWeaponComponent>(GetOwningPlayerPawn());

    if (!PlayerWeaponComponent) {
        return false;
    }

    return PlayerWeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool UShooterPlayerHUDWidget::GetCurrentWeaponAmmoText(
    FText &AmmoText
) const
{
    FAmmoData AmmoData;
    if (!GetCurrentWeaponAmmoData(AmmoData)) {
        return false;
    }

    FString BulletsString = FString::FromInt(AmmoData.BulletsInClip);
    FString SpareBulletsString = AmmoData.bInfiniteAmmo ? TEXT("\u221E") : FString::FromInt(AmmoData.SpareBullets);
    FString AmmoDataString = FString::Printf(TEXT("%s / %s"), *BulletsString, *SpareBulletsString);
    AmmoText = FText::FromString(AmmoDataString);
    return true;
}
