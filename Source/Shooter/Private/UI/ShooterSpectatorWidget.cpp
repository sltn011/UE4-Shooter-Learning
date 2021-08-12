// Shooter, All Rights Reserved


#include "UI/ShooterSpectatorWidget.h"

#include "Components/ShooterRespawnComponent.h"
#include "ShooterUtils.h"

bool UShooterSpectatorWidget::GetTimeUntilRespawn(
    int32 &TimeUntilRespawn
) const
{
    UShooterRespawnComponent *RespawnComponent = GetPlayerRespawnComponent();
    if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) {
        return false;
    }

    TimeUntilRespawn = RespawnComponent->GetTimeUntilRespawn();
    return true;
}

bool UShooterSpectatorWidget::IsRespawnInProgress(
) const
{
    UShooterRespawnComponent *RespawnComponent = GetPlayerRespawnComponent();
    return RespawnComponent && RespawnComponent->IsRespawnInProgress();
}

UShooterRespawnComponent *UShooterSpectatorWidget::GetPlayerRespawnComponent(
) const
{
    return ShooterUtils::GetPlayerComponentByClass<UShooterRespawnComponent>(GetOwningPlayer());
}
