// Shooter, All Rights Reserved


#include "UI/ShooterGameDataWidget.h"

#include "ShooterGameModeBase.h"

bool UShooterGameDataWidget::GetRoundsDataText(
    FText &RoundNumText
) const
{
    int32 CurrentRound, MaxRounds;
    if (!GetCurrentRoundNum(CurrentRound) || !GetMaxRoundsNum(MaxRounds)) {
        return false;
    }

    FString RoundNumString = FString::Printf(TEXT("%d / %d"), CurrentRound, MaxRounds);
    RoundNumText = FText::FromString(RoundNumString);
    return true;
}

bool UShooterGameDataWidget::GetRoundTimeRemainigText(
    FText &TimeRemainingText
) const
{
    int32 RoundCountdownSeconds;
    if (!GetRoundSecondsRemaining(RoundCountdownSeconds)) {
        return false;
    }

    int32 Minutes = RoundCountdownSeconds / 60;
    int32 Seconds = RoundCountdownSeconds % 60;

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
