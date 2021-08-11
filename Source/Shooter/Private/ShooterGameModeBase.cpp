// Shooter, All Rights Reserved


#include "ShooterGameModeBase.h"

#include "AIController.h"
#include "Player/ShooterBaseCharacter.h"
#include "Player/ShooterPlayerController.h"
#include "Player/ShooterPlayerState.h"
#include "UI/ShooterGameHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterGameModeBase, All, All);

AShooterGameModeBase::AShooterGameModeBase(
) {

    DefaultPawnClass = AShooterBaseCharacter::StaticClass();
    PlayerControllerClass = AShooterPlayerController::StaticClass();
    PlayerStateClass = AShooterPlayerState::StaticClass();
    HUDClass = AShooterGameHUD::StaticClass();
}

void AShooterGameModeBase::StartPlay(
)
{
    Super::StartPlay();

    SpawnBots();
    InitTeamsInfo();

    CurrentRound = 1;
    StartRound();
}

UClass *AShooterGameModeBase::GetDefaultPawnClassForController_Implementation(
    AController *InController
)
{
    if (InController && InController->IsA<AAIController>()) {
        return AIPawnClass;
    }

    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AShooterGameModeBase::RegisterKill(
    AController *Killer,
    AController *Victim
)
{
    if (!Killer || !Victim) {
        return;
    }

    AShooterPlayerState *KillerState = Killer->GetPlayerState<AShooterPlayerState>();
    AShooterPlayerState *VictimState = Victim->GetPlayerState<AShooterPlayerState>();

    if (!KillerState || !VictimState) {
        return;
    }

    KillerState->AddKill();
    VictimState->AddDeath();
}

void AShooterGameModeBase::LogPlayersStatistics(
) const
{
    UWorld *World = GetWorld();
    if (!World) {
        return;
    }

    for (FConstControllerIterator It = World->GetControllerIterator(); It; ++It) {

        AController *Controller = It->Get();
        if (!Controller) {
            continue;
        }

        AShooterPlayerState *State = Controller->GetPlayerState<AShooterPlayerState>();
        if (!State) {
            continue;
        }

        State->LogStatistics();
    }
}

int32 AShooterGameModeBase::GetRoundSecondsRemaining(
) const
{
    return RoundCountdown;
}

int32 AShooterGameModeBase::GetRoundNumber(
) const
{
    return CurrentRound;
}

int32 AShooterGameModeBase::GetNumberOfRounds(
) const
{
    return GameData.NumberOfRounds;
}

FGameData AShooterGameModeBase::GetGameData(
) const
{
    return GameData;
}

void AShooterGameModeBase::SpawnBots(
)
{
    UWorld *World = GetWorld();
    if (!World) {
        return;
    }

    // One player is always player
    for (int32 i = 0; i < GameData.NumberOfPlayers - 1; ++i) {

        FActorSpawnParameters SpawnParameters;
        SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AAIController *SpawnedController = World->SpawnActor<AAIController>(AIControllerClass, SpawnParameters);
        RestartPlayer(SpawnedController);
    }
}

void AShooterGameModeBase::StartRound(
)
{
    RoundCountdown = GameData.RoundTimeInSeconds;
    UE_LOG(LogShooterGameModeBase, Display, TEXT("Round %d started!"), CurrentRound);
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &AShooterGameModeBase::UpdateGameTimer, 1.0f, true);
}

void AShooterGameModeBase::UpdateGameTimer(
)
{
    if (--RoundCountdown == 0) {

        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        UE_LOG(LogShooterGameModeBase, Display, TEXT("Round %d ended!"), CurrentRound);
        
        if (++CurrentRound <= GameData.NumberOfRounds) {
            ResetPlayers();
            StartRound();
        }
        else {
            --CurrentRound; // Dec by 1 to not have round number bigger than max rounds on screen
            UE_LOG(LogShooterGameModeBase, Display, TEXT("Game over!"), CurrentRound);
            LogPlayersStatistics();
        }
    }
}

void AShooterGameModeBase::ResetPlayer(
    AController *Controller
) 
{
    if (Controller && Controller->GetPawn()) {
        Controller->GetPawn()->Reset();
    }

    RestartPlayer(Controller);
    SetPlayerColorFromState(Controller);
}

void AShooterGameModeBase::ResetPlayers(
)
{
    UWorld *World = GetWorld();
    if (!World) {
        return;
    }

    for (FConstControllerIterator It = World->GetControllerIterator(); It; ++It) {
        ResetPlayer(It->Get());
    }
}

void AShooterGameModeBase::InitTeamsInfo(
)
{
    UWorld *World = GetWorld();
    if (!World) {
        return;
    }

    int32 TeamID = 1;
    for (FConstControllerIterator It = World->GetControllerIterator(); It; ++It) {
        
        AController *Controller = It->Get();
        if (!Controller) {
            continue;
        }

        AShooterPlayerState *PlayerState = Controller->GetPlayerState<AShooterPlayerState>();
        if (!PlayerState) {
            continue;
        }

        FLinearColor TeamColor;
        if (!GetColorFromTeamID(TeamID, TeamColor)) {
            TeamColor = GameData.DefaultTeamColor;
        }

        PlayerState->TeamID = TeamID;
        PlayerState->TeamColor = TeamColor;
        SetPlayerColorFromState(Controller);

        TeamID = TeamID == 1 ? 2 : 1;
    }
}

bool AShooterGameModeBase::GetColorFromTeamID(
    int32 TeamID,
    FLinearColor &TeamColor
)
{
    if (!GameData.TeamColors.IsValidIndex(TeamID - 1)) {
        UE_LOG(LogShooterGameModeBase, Warning, TEXT("No color assigned for team with ID %d!"), TeamID);
        return false;
    }
    
    TeamColor = GameData.TeamColors[TeamID - 1];
    return true;
}

void AShooterGameModeBase::SetPlayerColorFromState(
    AController *Controller
)
{
    if (!Controller) {
        return;
    }

    AShooterPlayerState *PlayerState = Controller->GetPlayerState<AShooterPlayerState>();
    if (!PlayerState) {
        return;
    }

    AShooterBaseCharacter *Character = Cast<AShooterBaseCharacter>(Controller->GetPawn());
    if (!Character) {
        return;
    }

    Character->SetPlayerColor(PlayerState->TeamColor);
}
