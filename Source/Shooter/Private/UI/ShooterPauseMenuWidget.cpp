// Shooter, All Rights Reserved


#include "UI/ShooterPauseMenuWidget.h"

#include "Components/Button.h"
#include "ShooterGameModeBase.h"

bool UShooterPauseMenuWidget::Initialize(
)
{
    bool ParentVal = Super::Initialize();

    if (UnpauseButton) {
        UnpauseButton->OnClicked.AddDynamic(this, &UShooterPauseMenuWidget::OnUnpause);
    }

    return ParentVal;
}

void UShooterPauseMenuWidget::OnUnpause(
)
{
    UWorld *World = GetWorld();
    if (!World) {
        return;
    }

    AShooterGameModeBase *GameMode = World->GetAuthGameMode<AShooterGameModeBase>();
    if (!GameMode) {
        return;
    }

    EShooterGameState CurrentState = GameMode->GetShooterGameState();
    if (CurrentState == EShooterGameState::Paused) {
        GameMode->ClearPause();
    }
}
