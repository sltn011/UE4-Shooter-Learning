// Shooter, All Rights Reserved


#include "UI/ShooterGameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Player/ShooterPlayerController.h"
#include "ShooterGameModeBase.h"
#include "UI/ShooterBaseWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterGameHUD, All, All);

void AShooterGameHUD::DrawHUD(
)
{
    Super::DrawHUD();
}

EShooterGameUIOverlay AShooterGameHUD::GetUIOverlayWidget(
) const
{
    return CurrentUIOverlay;
}

void AShooterGameHUD::SetUIOverlayWidget(
    EShooterGameUIOverlay UIOverlay
)
{
    if (!CanUIOverlayBeSet(UIOverlay)) {
        return;
    }

    if (CurrentWidget) {
        CurrentWidget->RemoveFromViewport();
    }

    if (UIOverlay != EShooterGameUIOverlay::NONE) {
        CurrentWidget = UIOverlaysWidgetsMap[UIOverlay];
        if (CurrentWidget) {
            CurrentWidget->AddToViewport();
            UShooterBaseWidget *AnimatedCurrentWidget = Cast<UShooterBaseWidget>(CurrentWidget);
            if (AnimatedCurrentWidget) {
                AnimatedCurrentWidget->Show();
            }
        }
    }
    else {
        OnGameStateChange(EShooterGameState::InProgress); // Set InProgress widget back - game mode is already set to it at this point
    }

    CurrentUIOverlay = UIOverlay;
    OnGameUIOverlaySet.Broadcast(CurrentUIOverlay);
}

void AShooterGameHUD::BeginPlay(
)
{
    Super::BeginPlay();

    UWorld *World = GetWorld();
    check(World);

    AShooterGameModeBase *GameMode = World->GetAuthGameMode<AShooterGameModeBase>();
    check(GameMode);

    GameMode->OnGameStateChange.AddUObject(this, &AShooterGameHUD::OnGameStateChange);

    CreateWidgets();
}

void AShooterGameHUD::CreateWidgets(
)
{
    UWorld *World = GetWorld();
    if (!World) {
        return;
    }

    if (GameWidgetClass) {
        StateWidgetsMap.Add(EShooterGameState::InProgress, CreateWidget(World, GameWidgetClass));
    }

    if (PauseWidgetClass) {
        StateWidgetsMap.Add(EShooterGameState::Paused, CreateWidget(World, PauseWidgetClass));
    }

    if (GameOverWidgetClass) {
        StateWidgetsMap.Add(EShooterGameState::GameOver, CreateWidget(World, GameOverWidgetClass));
    }

    if (StatsTableWidgetClass) {
        UIOverlaysWidgetsMap.Add(EShooterGameUIOverlay::PlayersStatsTable, CreateWidget(World, StatsTableWidgetClass));
    }
}

void AShooterGameHUD::OnGameStateChange(
    EShooterGameState NewState
)
{
    if (!StateWidgetsMap.Contains(NewState)) {
        return;
    }

    if (CurrentWidget) {
        CurrentWidget->RemoveFromViewport();
    }

    CurrentWidget = StateWidgetsMap[NewState];

    if (CurrentWidget) {
        CurrentWidget->AddToViewport();
        UShooterBaseWidget *AnimatedCurrentWidget = Cast<UShooterBaseWidget>(CurrentWidget);
        if (AnimatedCurrentWidget) {
            AnimatedCurrentWidget->Show();
        }
    }
}

bool AShooterGameHUD::GetCurrentGameState(
    EShooterGameState &GameState
) const
{
    UWorld *World = GetWorld();
    if (!World) {
        return false;
    }

    AShooterGameModeBase *GameMode = World->GetAuthGameMode<AShooterGameModeBase>();
    if (!GameMode) {
        return false;
    }

    GameState = GameMode->GetShooterGameState();
    return true;
}

bool AShooterGameHUD::CanUIOverlayBeSet(
    EShooterGameUIOverlay UIOverlay
)
{
    EShooterGameState CurrentGameState;
    if (!GetCurrentGameState(CurrentGameState)) {
        return false;
    }

    if (CurrentGameState != EShooterGameState::InProgress) {
        return false;
    }

    if (CurrentUIOverlay == UIOverlay) {
        return false;
    }

    return UIOverlay == EShooterGameUIOverlay::NONE || UIOverlaysWidgetsMap.Contains(UIOverlay);
}
