// Shooter, All Rights Reserved


#include "Menu/ShooterMenuGameModeBase.h"

#include "Menu/ShooterMenuPlayerController.h"
#include "Menu/ShooterMenuPlayerPawn.h"
#include "Menu/UI/ShooterMenuHUD.h"

AShooterMenuGameModeBase::AShooterMenuGameModeBase(
)
{
    PlayerControllerClass = AShooterMenuPlayerController::StaticClass();
    DefaultPawnClass = AShooterMenuPlayerPawn::StaticClass();
    HUDClass = AShooterMenuHUD::StaticClass();
}