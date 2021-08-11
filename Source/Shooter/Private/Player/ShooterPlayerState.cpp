// Shooter, All Rights Reserved


#include "Player/ShooterPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterPlayerState, All, All);

void AShooterPlayerState::AddKill(
)
{
    ++KillsNum;
}

int32 AShooterPlayerState::GetKills(
) const
{
    return KillsNum;
}

void AShooterPlayerState::ResetKills(
)
{
    KillsNum = 0;
}

void AShooterPlayerState::AddDeath(
) 
{
    ++DeathsNum;
}

int32 AShooterPlayerState::GetDeaths(
) const
{
    return DeathsNum;
}

void AShooterPlayerState::ResetDeaths(
)
{
    DeathsNum = 0;
}

void AShooterPlayerState::ResetStatistics(
)
{
    ResetKills();
    ResetDeaths();
}

void AShooterPlayerState::LogStatistics(
) const
{
    UE_LOG(
        LogShooterPlayerState, Display,
        TEXT("Player: %s, TeamID: %d, Kills: %d, Deaths: %d"),
        *GetName(), TeamID, KillsNum, DeathsNum
    );
}
