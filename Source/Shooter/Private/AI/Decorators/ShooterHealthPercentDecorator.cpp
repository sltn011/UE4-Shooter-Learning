// Shooter, All Rights Reserved


#include "AI/Decorators/ShooterHealthPercentDecorator.h"

#include "AIController.h"
#include "Components/ShooterHealthComponent.h"
#include "ShooterUtils.h"

UShooterHealthPercentDecorator::UShooterHealthPercentDecorator(
)
{
    NodeName = "Health Percent";
}

bool UShooterHealthPercentDecorator::CalculateRawConditionValue(
    UBehaviorTreeComponent &OwnerComp,
    uint8 *NodeMemory
) const
{
    AAIController *Controller = OwnerComp.GetAIOwner();
    if (!Controller) {
        return false;
    }

    UShooterHealthComponent *HealthComponent = ShooterUtils::GetPlayerComponentByClass<UShooterHealthComponent>(Controller->GetPawn());
    if (!HealthComponent || HealthComponent->IsDead()) {
        return false;
    }

    return HealthComponent->GetHealthPercent() <= HealthThreshold;  
}
