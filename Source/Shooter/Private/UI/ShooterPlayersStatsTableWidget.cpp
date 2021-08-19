// Shooter, All Rights Reserved


#include "UI/ShooterPlayersStatsTableWidget.h"

#include "Components/VerticalBox.h"
#include "EngineUtils.h"
#include "Player/ShooterPlayerState.h"
#include "ShooterGameModeBase.h"
#include "UI/ShooterGameHUD.h"
#include "UI/ShooterPlayerStatRowWidget.h"

bool UShooterPlayersStatsTableWidget::Initialize(
)
{
    bool ParentVal = Super::Initialize();

    UWorld *World = GetWorld();
    if (!World) {
        return false;
    }

    AShooterGameModeBase *GameMode = World->GetAuthGameMode<AShooterGameModeBase>();
    if (GameMode) {
        GameMode->OnGameStateChange.AddUObject(this, &UShooterPlayersStatsTableWidget::OnGameStateChange);
    }

    APlayerController *OwnerController = GetOwningPlayer();
    if (OwnerController) {
        AShooterGameHUD *OwnerHUD = OwnerController->GetHUD<AShooterGameHUD>();
        if (OwnerHUD) {
            OwnerHUD->OnGameUIOverlaySet.AddUObject(this, &UShooterPlayersStatsTableWidget::OnGameUIOverlaySet);
        }
    }

    return ParentVal;
}

void UShooterPlayersStatsTableWidget::OnGameStateChange(
    EShooterGameState NewState
)
{
    if (NewState == EShooterGameState::GameOver) {
        if (ShouldBeCreated()) {
            CreateStatsTable();
        }
    }
}

void UShooterPlayersStatsTableWidget::OnGameUIOverlaySet(
    EShooterGameUIOverlay UIOverlay
)
{
    if (UIOverlay == EShooterGameUIOverlay::PlayersStatsTable) {
        if (ShouldBeCreated()) {
            CreateStatsTable();
        }
    }
}

void UShooterPlayersStatsTableWidget::CreateStatsTable(
)
{
    UWorld *World = GetWorld();
    if (!World || !PlayerStatsBox || !PlayerStatRowWidgetClass) {
        return;
    }

    for (AShooterPlayerState *PlayerState : TActorRange<AShooterPlayerState>(World)) {
        if (!PlayerState) {
            continue;
        }

        UShooterPlayerStatRowWidget *StatRowWidget = CreateWidget<UShooterPlayerStatRowWidget>(this, PlayerStatRowWidgetClass);
        if (!StatRowWidget) {
            continue;
        }

        StatRowWidget->SetSpectatedPlayerState(PlayerState);

        PlayerStatsBox->AddChild(StatRowWidget);
    }

    bIsCreated = true;
}

bool UShooterPlayersStatsTableWidget::ShouldBeCreated(
)
{
    return !bIsCreated;
}
