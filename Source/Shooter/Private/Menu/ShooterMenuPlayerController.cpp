// Shooter, All Rights Reserved


#include "Menu/ShooterMenuPlayerController.h"

void AShooterMenuPlayerController::BeginPlay(
)
{
    Super::BeginPlay();

    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
}