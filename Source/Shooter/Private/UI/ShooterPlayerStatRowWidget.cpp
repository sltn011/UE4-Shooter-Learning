// Shooter, All Rights Reserved


#include "UI/ShooterPlayerStatRowWidget.h"

#include "Player/ShooterPlayerState.h"
#include "ShooterGameModeBase.h"

void UShooterPlayerStatRowWidget::SetSpectatedPlayerState(
    AShooterPlayerState *SpectatedState
)
{
    SpectatedPlayerState = SpectatedState;
}

bool UShooterPlayerStatRowWidget::GetPlayerName(
    FString &PlayerName
) const
{
    if (!SpectatedPlayerState) {
        return false;
    }
    
    PlayerName = SpectatedPlayerState->GetPlayerName();
    return true;
}

bool UShooterPlayerStatRowWidget::GetPlayerKillsNum(
    int32 &KillsNum
) const
{
    if (!SpectatedPlayerState) {
        return false;
    }

    KillsNum = SpectatedPlayerState->GetKills();
    return true;
}

bool UShooterPlayerStatRowWidget::GetPlayerDeathsNum(
    int32 &DeathsNum
) const
{
    if (!SpectatedPlayerState) {
        return false;
    }

    DeathsNum = SpectatedPlayerState->GetDeaths();
    return true;
}

bool UShooterPlayerStatRowWidget::GetPlayerTeamColor(
    FLinearColor &Color
) const
{
    if (!SpectatedPlayerState) {
        return false;
    }

    AShooterGameModeBase *GameMode = GetGameMode();
    if (!GameMode) {
        return false;
    }

    return GameMode->GetTeamColor(SpectatedPlayerState->TeamID, Color);
}

bool UShooterPlayerStatRowWidget::GetTextColor(
    FLinearColor &Color
) const
{
    AShooterPlayerState *OwnerPlayerState = GetOwningPlayerState<AShooterPlayerState>();
    if (!OwnerPlayerState || !SpectatedPlayerState) {
        return false;
    }

    Color = OwnerPlayerState == SpectatedPlayerState ? CurrentPlayerTextColor : DefaultPlayerTextColor;
    return true;
}

AShooterGameModeBase *UShooterPlayerStatRowWidget::GetGameMode(
) const
{
    UWorld *World = GetWorld();
    if (!World) {
        return nullptr;
    }

    return World->GetAuthGameMode<AShooterGameModeBase>();
}
