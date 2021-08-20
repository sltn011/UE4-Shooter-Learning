// Shooter, All Rights Reserved


#include "Menu/ShooterMenuGameModeBase.h"

#include "Menu/ShooterMenuPlayerController.h"
#include "Menu/UI/ShooterMenuHUD.h"

AShooterMenuGameModeBase::AShooterMenuGameModeBase(
)
{
    PlayerControllerClass = AShooterMenuPlayerController::StaticClass();
    HUDClass = AShooterMenuHUD::StaticClass();
}