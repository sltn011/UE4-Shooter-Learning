// Shooter, All Rights Reserved


#include "GameModes/ShooterGameModeCS.h"

#include "EngineUtils.h"
#include "Player/ShooterPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterGameModeCS, All, All);

AShooterGameModeCS::AShooterGameModeCS(
)
{
    GameData.NumberOfPlayers = 10;
    GameData.NumberOfRounds = 15;
    GameData.RoundTimeInSeconds = 90;

    bRespawningEnabled = false;
}

void AShooterGameModeCS::ShuffleSpawnPoints(
)
{
    int32 Team1Players = 0, Team2Players = 0;
    CountTeamsPlayers(Team1Players, Team2Players);

    TArray<int32> Team1Spawns, Team2Spawns;
    Team1Spawns.SetNumZeroed(Team1Players);
    Team2Spawns.SetNumZeroed(Team2Players);

    auto FillArray = [](TArray<int32> &Array) {
        for (int32 i = 0; i < Array.Num(); ++i) {
            Array[i] = i + 1;
        }
    };

    FillArray(Team1Spawns);
    FillArray(Team2Spawns);

    ShuffleArray(Team1Spawns);
    ShuffleArray(Team2Spawns);

    SetTeamSpawnPoints(1, Team1Spawns);
    SetTeamSpawnPoints(2, Team2Spawns);
}

void AShooterGameModeCS::SetTeamSpawnPoints(
    int32 TeamID,
    TArray<int32> const &SpawnPointsIDs
)
{
    int32 Cnt = 0;
    for (AController *Player : TActorRange<AController>(GetWorld())) {
        if (!Player) {
            return;
        }

        AShooterPlayerState *PlayerState = Player->GetPlayerState<AShooterPlayerState>();
        if (!PlayerState) {
            return;
        }

        if (PlayerState->TeamID == TeamID) {
            PlayerState->PlayerSpawnPointNum = SpawnPointsIDs.IsValidIndex(Cnt) ? SpawnPointsIDs[Cnt++] : 0;
        }
    }
}

void AShooterGameModeCS::SwapTeams(
)
{
    for (AController *Player : TActorRange<AController>(GetWorld())) {
        if (!Player) {
            continue;
        }

        AShooterPlayerState *PlayerState = Player->GetPlayerState<AShooterPlayerState>();
        if (!PlayerState) {
            continue;
        }

        int32 NewTeamID = PlayerState->TeamID == 1 ? 2 : 1;
        PlayerState->TeamID = NewTeamID;

        FLinearColor NewTeamColor;
        if (GetTeamColor(NewTeamID, NewTeamColor)) {
            PlayerState->TeamColor = NewTeamColor;
            SetPlayerColorFromState(Player);
        }
    }

    Swap(Team1Score, Team2Score);
}

int32 AShooterGameModeCS::GetTeamPlayerCount(
    int32 TeamID,
    bool bAliveOnly
)
{
    switch (TeamID) {
    case 1:
        return bAliveOnly ? Team1AlivePlayersNum : CountTeamPlayers(TeamID);

    case 2:
        return bAliveOnly ? Team2AlivePlayersNum : CountTeamPlayers(TeamID);

    default:
        return 0;
    }
}

bool AShooterGameModeCS::GetTeamScore(
    int32 TeamID,
    int32 &TeamScore
)
{
    switch (TeamID) {
    case 1:
        TeamScore = Team1Score;
        return true;

    case 2:
        TeamScore = Team2Score;
        return true;

    default:
        return false;
    }

    return false;
}

void AShooterGameModeCS::RegisterKill(AController *Killer, AController *Victim)
{
    Super::RegisterKill(Killer, Victim);

    if (!Victim) {
        return;
    }

    AShooterPlayerState *VictimState = Victim->GetPlayerState<AShooterPlayerState>();
    if (!VictimState) {
        return;
    }
    VictimState->TeamID == 1 ? --Team1AlivePlayersNum : --Team2AlivePlayersNum;

    if (Team1AlivePlayersNum == 0 || Team2AlivePlayersNum == 0) {
        RoundCountdown = EndRoundTimeout;
    }

    UE_LOG(LogShooterGameModeCS, Display, TEXT("%d %d"), Team1AlivePlayersNum, Team2AlivePlayersNum);
}

void AShooterGameModeCS::OnRoundStart(
)
{
    CountTeamsPlayers(Team1AlivePlayersNum, Team2AlivePlayersNum);
    Super::OnRoundStart();
}

void AShooterGameModeCS::OnRoundEnd(
)
{
    UpdateScore();

    SwapTeams();
    ShuffleSpawnPoints();

    Super::OnRoundEnd();
}

void AShooterGameModeCS::OnGameEnd(
)
{
    UpdateScore();
    Super::OnGameEnd();
}

void AShooterGameModeCS::InitTeamsInfo()
{
    Super::InitTeamsInfo();
    ShuffleSpawnPoints();
}

AActor *AShooterGameModeCS::FindPlayerStart_Implementation(
    AController *Player,
    FString const &IncomingName
)
{
    if (!Player) {
        return nullptr;
    }

    AShooterPlayerState const *PlayerState = Player->GetPlayerState<AShooterPlayerState>();
    if (!PlayerState) {
        return nullptr;
    }

    int32 TeamID = PlayerState->TeamID;
    int32 PlayerSpawnPoint = PlayerState->PlayerSpawnPointNum;
    FString TeamIDTag = FString::Printf(TEXT("%d_%d"), TeamID, PlayerSpawnPoint);

    return Super::FindPlayerStart_Implementation(Player, TeamIDTag);
}

int32 AShooterGameModeCS::CountTeamPlayers(
    int32 TeamID
)
{
    int32 TeamPlayersCnt = 0;

    for (AController *Player : TActorRange<AController>(GetWorld())) {
        if (!Player) {
            continue;
        }

        AShooterPlayerState *PlayerState = Player->GetPlayerState<AShooterPlayerState>();
        if (!PlayerState) {
            continue;
        }

        if (PlayerState->TeamID == TeamID) {
            ++TeamPlayersCnt;
        }
    }

    return TeamPlayersCnt;
}

void AShooterGameModeCS::CountTeamsPlayers(
    int32 &Team1Players,
    int32 &Team2Players
)
{
    Team1Players = CountTeamPlayers(1);
    Team2Players = CountTeamPlayers(2);
}

void AShooterGameModeCS::ShuffleArray(
    TArray<int32> &Array
)
{
    for (int32 i = 0; i < Array.Num(); ++i) {
        int32 IndexA = FMath::Rand() % Array.Num();
        int32 IndexB = FMath::Rand() % Array.Num();
        Array.Swap(IndexA, IndexB);
    }
}

void AShooterGameModeCS::UpdateScore(
)
{
    // Do nothing in case of draw
    if (Team1AlivePlayersNum != Team2AlivePlayersNum) {
        if (Team1AlivePlayersNum == 0) {
            ++Team2Score;
        }
        else if (Team2AlivePlayersNum == 0) {
            ++Team1Score;
        }
        else {
            Team1AlivePlayersNum > Team2AlivePlayersNum ? ++Team1Score : ++Team2Score;
        }
    }
}
