// Shooter, All Rights Reserved


#include "UI/ShooterPlayerHUDWidget.h"

#include "Components/ShooterHealthComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "ShooterGameModeBase.h"
#include "ShooterUtils.h"

bool UShooterPlayerHUDWidget::Initialize(
)
{
    bool ParentResult = Super::Initialize();

    APlayerController *PlayerController = GetOwningPlayer();
    if (!PlayerController) {
        return false;
    }
    PlayerController->GetOnNewPawnNotifier().AddUObject(this, &UShooterPlayerHUDWidget::OnNewPawn);
    OnNewPawn(GetOwningPlayerPawn());

    return ParentResult;
}

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
    FString SpareBulletsString = AmmoData.bInfiniteAmmo ? TEXT("\u221E") : FString::FromInt(AmmoData.SpareAmmo);
    FString AmmoDataString = FString::Printf(TEXT("%s / %s"), *BulletsString, *SpareBulletsString);
    AmmoText = FText::FromString(AmmoDataString);
    return true;
}

void UShooterPlayerHUDWidget::OnHealthChanged(
    float NewHealth,
    float HealthDelta
)
{
    if (HealthDelta < 0.0f) {
        OnTakeDamage(); // BP implementable event

        if (!IsAnimationPlaying(OnDamaged)) {
            PlayAnimation(OnDamaged);
        }
    }
}

void UShooterPlayerHUDWidget::OnNewPawn(
    APawn *NewPawn
)
{
    UShooterHealthComponent *PlayerHealthComponent = ShooterUtils::GetPlayerComponentByClass<UShooterHealthComponent>(NewPawn);
    if (!PlayerHealthComponent || PlayerHealthComponent->OnHealthChanged.IsBoundToObject(this)) {
        return;
    }

    PlayerHealthComponent->OnHealthChanged.AddUObject(this, &UShooterPlayerHUDWidget::OnHealthChanged);
}
