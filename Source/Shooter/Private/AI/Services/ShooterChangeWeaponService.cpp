// Shooter, All Rights Reserved


#include "AI/Services/ShooterChangeWeaponService.h"

#include "AIController.h"
#include "Components/ShooterWeaponComponent.h"
#include "ShooterUtils.h"

UShooterChangeWeaponService::UShooterChangeWeaponService(
)
{
    NodeName = "Change Weapon";
}

void UShooterChangeWeaponService::TickNode(
    UBehaviorTreeComponent &OwnerComp,
    uint8 *NodeMemory,
    float DeltaSeconds
)
{
    AAIController *Controller = OwnerComp.GetAIOwner();
    if (!Controller) {
        return;
    }

    UShooterWeaponComponent *WeaponComponent = ShooterUtils::GetPlayerComponentByClass<UShooterWeaponComponent>(Controller->GetPawn());
    if (!WeaponComponent) {
        return;
    }

    if (ShouldChange()) {
        WeaponComponent->EquipNextWeapon();
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}

bool UShooterChangeWeaponService::ShouldChange(
) const
{
    return ChangeWeaponProbability > 0.0f && FMath::FRand() <= ChangeWeaponProbability;
}
