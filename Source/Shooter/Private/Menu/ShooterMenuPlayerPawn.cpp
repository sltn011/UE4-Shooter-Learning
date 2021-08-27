// Shooter, All Rights Reserved


#include "Menu/ShooterMenuPlayerPawn.h"

#include "Camera/CameraComponent.h"

AShooterMenuPlayerPawn::AShooterMenuPlayerPawn(
)
{
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    SetRootComponent(CameraComponent);
}
