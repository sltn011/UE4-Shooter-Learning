// Shooter, All Rights Reserved


#include "Menu/ShooterMenuPlayerController.h"

void AShooterMenuPlayerController::BeginPlay(
)
{
    Super::BeginPlay();

    SetInputMode(FInputModeGameAndUI());
    bShowMouseCursor = true;
}