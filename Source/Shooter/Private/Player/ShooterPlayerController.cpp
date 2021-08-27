// Shooter, All Rights Reserved


#include "Player/ShooterPlayerController.h"

#include "Components/ShooterRespawnComponent.h"
#include "ShooterGameInstance.h"
#include "ShooterGameModeBase.h"
#include "UI/ShooterGameHUD.h"


AShooterPlayerController::AShooterPlayerController(
)
{
    RespawnComponent = CreateDefaultSubobject<UShooterRespawnComponent>(TEXT("RespawnComponent"));
}

void AShooterPlayerController::BeginPlay(
)
{
    Super::BeginPlay();

    UWorld *World = GetWorld();
    check(World);

    AShooterGameModeBase *GameMode = World->GetAuthGameMode<AShooterGameModeBase>();
    if (GameMode) {
        GameMode->OnGameStateChange.AddUObject(this, &AShooterPlayerController::OnGameStateChange);
    }

    AShooterGameHUD *GameHUD = GetHUD<AShooterGameHUD>();
    if (GameHUD) {
        GameHUD->OnGameUIOverlaySet.AddUObject(this, &AShooterPlayerController::OnGameUIOverlaySet);
    }
}

void AShooterPlayerController::SetupInputComponent(
)
{
    Super::SetupInputComponent();

    if (!InputComponent) {
        return;
    }

    InputComponent->BindAction("TogglePause", EInputEvent::IE_Pressed, this, &AShooterPlayerController::OnTogglePause).bExecuteWhenPaused = true;
    InputComponent->BindAction("ToggleStatsTable", EInputEvent::IE_Pressed, this, &AShooterPlayerController::OnToggleStatsTable);
    InputComponent->BindAction("ToggleStatsTable", EInputEvent::IE_Released, this, &AShooterPlayerController::OnToggleStatsTable);
    InputComponent->BindAction("ToggleSound", EInputEvent::IE_Pressed, this, &AShooterPlayerController::OnToggleSound).bExecuteWhenPaused = true;
}

void AShooterPlayerController::OnTogglePause(
)
{
    AShooterGameModeBase *GameMode = GetGameMode();
    if (!GameMode) {
        return;
    }

    EShooterGameState CurrentState;
    if (!GetCurrentGameState(CurrentState)) {
        return;
    }

    switch (CurrentState) {

    case EShooterGameState::GameOver:
        return;

    case EShooterGameState::Paused:
        GameMode->ClearPause();
        break;

    case EShooterGameState::InProgress:
        GameMode->SetPause(this);
        break;

    default:
        break;

    }
}

void AShooterPlayerController::OnToggleStatsTable(
)
{
    EShooterGameState CurrentState;
    if (!GetCurrentGameState(CurrentState)) {
        return;
    }

    if (CurrentState == EShooterGameState::InProgress) {
        AShooterGameHUD *PlayerHUD = GetHUD<AShooterGameHUD>();
        EShooterGameUIOverlay CurrentOverlay = PlayerHUD->GetUIOverlayWidget();
        if (CurrentOverlay == EShooterGameUIOverlay::NONE) {
            PlayerHUD->SetUIOverlayWidget(EShooterGameUIOverlay::PlayersStatsTable);
        }
        else {
            PlayerHUD->SetUIOverlayWidget(EShooterGameUIOverlay::NONE);
        }
    }
}

void AShooterPlayerController::OnGameStateChange(
    EShooterGameState NewState
)
{
    if (NewState == EShooterGameState::InProgress) {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else {
        SetInputMode(FInputModeGameAndUI());
        bShowMouseCursor = true;
    }
}

void AShooterPlayerController::OnGameUIOverlaySet(
    EShooterGameUIOverlay UIOverlay
)
{
    //if (UIOverlay == EShooterGameUIOverlay::PlayersStatsTable) {
    //    SetInputMode(FInputModeGameAndUI());
    //    bShowMouseCursor = true;
    //}
    //else {
    //    SetInputMode(FInputModeGameOnly());
    //    bShowMouseCursor = false;
    //}
}

void AShooterPlayerController::OnToggleSound(
)
{
    UWorld *World = GetWorld();
    if (!World) {
        return;
    }

    UShooterGameInstance *GameInstance = World->GetGameInstance<UShooterGameInstance>();
    if (!GameInstance) {
        return;
    }

    GameInstance->ToggleSound();
}

bool AShooterPlayerController::GetCurrentGameState(
    EShooterGameState &State
) const
{
    AShooterGameModeBase *GameMode = GetGameMode();
    if (!GameMode) {
        return false;
    }

    State = GameMode->GetShooterGameState();
    return true;
}

AShooterGameModeBase *AShooterPlayerController::GetGameMode() const
{
    UWorld *World = GetWorld();
    if (!World) {
        return nullptr;
    }

    return World->GetAuthGameMode<AShooterGameModeBase>();
}
