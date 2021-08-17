// Shooter, All Rights Reserved


#include "UI/ShooterGameDataWidget.h"

#include "ShooterGameModeBase.h"

bool UShooterGameDataWidget::GetRoundsData(
    int32 &CurrentRoundsNum,
    int32 &MaxRoundsNum
) const
{
    return GetCurrentRoundNum(CurrentRoundsNum) && GetMaxRoundsNum(MaxRoundsNum);
}

bool UShooterGameDataWidget::GetRoundTimeRemaining(
    int32 &MinutesRemaining,
    int32 &SecondsRemaining
) const
{
    int32 RoundCountdownSeconds;
    if (!GetRoundSecondsRemaining(RoundCountdownSeconds)) {
        return false;
    }

    MinutesRemaining = RoundCountdownSeconds / 60;
    SecondsRemaining = RoundCountdownSeconds % 60;
    return true;
}

bool UShooterGameDataWidget::GetRoundTimeRemainingText(
    FText &TimeRemainingText
) const
{
    int32 Minutes, Seconds;
    if (!GetRoundTimeRemaining(Minutes, Seconds)) {
        return false;
    }

    FString MinutesString = Minutes > 9 ? FString::Printf(TEXT("%d"), Minutes) : FString::Printf(TEXT("0%d"), Minutes);
    FString SecondsString = Seconds > 9 ? FString::Printf(TEXT("%d"), Seconds) : FString::Printf(TEXT("0%d"), Seconds);

    FString TimeRemainingString = FString::Printf(TEXT("%s : %s"), *MinutesString, *SecondsString);

    TimeRemainingText = FText::FromString(TimeRemainingString);
    return true;
}

AShooterGameModeBase *UShooterGameDataWidget::GetGameMode(
) const
{
    UWorld *World = GetWorld();
    if (!World) {
        return false;
    }

    return World->GetAuthGameMode<AShooterGameModeBase>();
}

bool UShooterGameDataWidget::GetCurrentRoundNum(
    int32 &RoundNum
) const
{
    AShooterGameModeBase *GameMode = GetGameMode();
    if (!GameMode) {
        return false;
    }

    RoundNum = GameMode->GetRoundNumber();
    return true;
}

bool UShooterGameDataWidget::GetMaxRoundsNum(
    int32 &MaxRoundsNum
) const
{
    AShooterGameModeBase *GameMode = GetGameMode();
    if (!GameMode) {
        return false;
    }

    MaxRoundsNum = GameMode->GetNumberOfRounds();
    return true;
}

bool UShooterGameDataWidget::GetRoundSecondsRemaining(
    int32 &SecondsRemaining
) const
{
    AShooterGameModeBase *GameMode = GetGameMode();
    if (!GameMode) {
        return false;
    }

    SecondsRemaining = GameMode->GetRoundSecondsRemaining();
    return true;
}
