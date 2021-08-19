// Shooter, All Rights Reserved


#include "ShooterGameModeBase.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Components/ShooterRespawnComponent.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Player/ShooterBaseCharacter.h"
#include "Player/ShooterPlayerController.h"
#include "Player/ShooterPlayerState.h"
#include "ShooterUtils.h"
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

    SpawnBotsControllers();
    InitTeamsInfo();
    ResetPlayers();

    CurrentRound = 1;
    OnRoundStart();

    SetShooterGameState(EShooterGameState::InProgress);
}

UClass *AShooterGameModeBase::GetDefaultPawnClassForController_Implementation(
    AController *Controller
)
{
    if (Controller && Controller->IsA<AAIController>()) {
        return AIPawnClass;
    }

    return Super::GetDefaultPawnClassForController_Implementation(Controller);
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

    if (IsRespawningEnabled()) {
        RespawnAfterDelay(Victim);
    }
}

bool AShooterGameModeBase::SetPause(
    APlayerController *PlayerController,
    FCanUnpause CanUnpauseDelegate
)
{
    bool ParentVal = Super::SetPause(PlayerController, CanUnpauseDelegate);

    if (ParentVal) {
        SetShooterGameState(EShooterGameState::Paused);
    }

    return ParentVal;
}

bool AShooterGameModeBase::ClearPause(
)
{
    bool ParentVal = Super::ClearPause();

    if (ParentVal) {
        SetShooterGameState(EShooterGameState::InProgress);
    }

    return ParentVal;
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

bool AShooterGameModeBase::GetTeamColor(
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

void AShooterGameModeBase::RespawnImmediate(
    AController *Controller
)
{
    if (!IsRespawningEnabled() || !Controller) {
        return;
    }

    ResetPlayer(Controller);
}

void AShooterGameModeBase::RespawnAfterDelay(
    AController *Controller
)
{
    if (!IsRespawningEnabled()) {
        return;
    }

    if (RoundCountdown <= GameData.RespawnTimeInSeconds) {
        return;
    }

    UShooterRespawnComponent *RespawnComponent = ShooterUtils::GetPlayerComponentByClass<UShooterRespawnComponent>(Controller);
    if (!RespawnComponent) {
        return;
    }

    RespawnComponent->RespawnRequest(GameData.RespawnTimeInSeconds);
}

bool AShooterGameModeBase::IsRespawningEnabled(
) const
{
    return bRespawningEnabled;
}

EShooterGameState AShooterGameModeBase::GetShooterGameState(
) const
{
    return ShooterGameState;
}

void AShooterGameModeBase::OnRoundStart(
)
{
    RoundCountdown = GameData.RoundTimeInSeconds;
    UE_LOG(LogShooterGameModeBase, Display, TEXT("Round %d started!"), CurrentRound);
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &AShooterGameModeBase::UpdateGameTimer, 1.0f, true);
}

void AShooterGameModeBase::OnRoundEnd(
)
{
    ResetPlayers();
    OnRoundStart();
}

void AShooterGameModeBase::OnGameEnd(
)
{
    --CurrentRound; // Dec by 1 to not have round number bigger than max rounds on screen
    GameOver();
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

void AShooterGameModeBase::SpawnBotsControllers(
)
{
    UWorld *World = GetWorld();
    if (!World) {
        return;
    }

    // One player is not bot
    for (int32 i = 0; i < GameData.NumberOfPlayers - 1; ++i) {

        FActorSpawnParameters SpawnParameters;
        SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AAIController *SpawnedController = World->SpawnActor<AAIController>(AIControllerClass, SpawnParameters);
        if (SpawnedController) {
            AShooterPlayerState *SpawnedState = SpawnedController->GetPlayerState<AShooterPlayerState>();
            if (SpawnedState) {
                SpawnedState->SetPlayerName(FString::Printf(TEXT("Bot Player %d"), i));
            }
        }
    }
}

void AShooterGameModeBase::SetShooterGameState(
    EShooterGameState NewGameState
)
{
    if (ShooterGameState == NewGameState) {
        return;
    }

    ShooterGameState = NewGameState;
    OnGameStateChange.Broadcast(ShooterGameState);
}

void AShooterGameModeBase::UpdateGameTimer(
)
{
    if (--RoundCountdown == 0) {

        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        UE_LOG(LogShooterGameModeBase, Display, TEXT("Round %d ended!"), CurrentRound);
        
        if (++CurrentRound <= GameData.NumberOfRounds) {
            OnRoundEnd();
        }
        else {
            OnGameEnd();
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

    AShooterPlayerState *PlayerState = Controller->GetPlayerState<AShooterPlayerState>();
    if (!PlayerState) {
        return;
    }
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

    int32 PlayerInTeam1ID = 1;
    int32 PlayerInTeam2ID = 1;

    int32 TeamID = FMath::RandBool() ? 1 : 2;
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
        if (!GetTeamColor(TeamID, TeamColor)) {
            TeamColor = GameData.DefaultTeamColor;
        }

        PlayerState->TeamID = TeamID;
        PlayerState->TeamColor = TeamColor;
        SetPlayerColorFromState(Controller);

        TeamID = TeamID == 1 ? 2 : 1;
    }
}

void AShooterGameModeBase::GameOver(
)
{
    UE_LOG(LogShooterGameModeBase, Display, TEXT("Game over!"), CurrentRound);
    LogPlayersStatistics();

    for (AShooterBaseCharacter *Character : TActorRange<AShooterBaseCharacter>(GetWorld())) {
        if (Character) {

            AController *CharacterController = Character->GetController<AController>();

            Character->TurnOff();
            Character->StopShooting();

            AAIController *AIController = Cast<AAIController>(CharacterController);
            if (AIController) {
                UBrainComponent *Brain = AIController->GetBrainComponent();
                if (Brain) {
                    Brain->Cleanup();
                }
            }
            else {
                Character->DisableInput(Cast<APlayerController>(CharacterController));
            }
        }
    }

    SetShooterGameState(EShooterGameState::GameOver);
}
