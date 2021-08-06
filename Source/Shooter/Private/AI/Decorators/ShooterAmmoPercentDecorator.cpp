// Shooter, All Rights Reserved


#include "AI/Decorators/ShooterAmmoPercentDecorator.h"

#include "AIController.h"
#include "Components/ShooterWeaponComponent.h"
#include "ShooterUtils.h"
#include "Weapon/ShooterBaseWeapon.h"

UShooterAmmoPercentDecorator::UShooterAmmoPercentDecorator(
)
{
    NodeName = "Ammo Percent";
}

bool UShooterAmmoPercentDecorator::CalculateRawConditionValue(
    UBehaviorTreeComponent &OwnerComp,
    uint8 *NodeMemory
) const
{
    if (!WeaponType) {
        return false;
    }

    AAIController *Controller = OwnerComp.GetAIOwner();

    UShooterWeaponComponent *WeaponComponent = ShooterUtils::GetPlayerComponentByClass<UShooterWeaponComponent>(Controller->GetPawn());
    if (!WeaponComponent) {
        return false;
    }

    FAmmoData DefaultAmmo, CurrentAmmo;
    if (!WeaponComponent->GetWeaponFullAmmoDataByClass(WeaponType, DefaultAmmo, CurrentAmmo)) {
        return false;
    }

    if (CurrentAmmo.bInfiniteAmmo) {
        return false;
    }

    int32 DefaultBullets = DefaultAmmo.BulletsInClip * DefaultAmmo.Clips + DefaultAmmo.BulletsInClip;
    int32 CurrentBullets = CurrentAmmo.SpareAmmo + CurrentAmmo.BulletsInClip;

    float AmmoPercent = static_cast<float>(CurrentBullets) / static_cast<float>(DefaultBullets);

    return AmmoPercent <= AmmoThreshold;
}
