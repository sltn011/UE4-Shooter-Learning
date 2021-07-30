// Shooter, All Rights Reserved


#include "Player/ShooterSpectatorPawn.h"

#include "Camera/CameraComponent.h"

AShooterSpectatorPawn::AShooterSpectatorPawn(
)
{
    SpectatorCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SpectatorCamera"));
    SpectatorCamera->bUsePawnControlRotation = true;
    SpectatorCamera->PostProcessSettings.ColorSaturation = FVector4{ 1.0f, 1.0f, 1.0f, 0.0f };
    SpectatorCamera->PostProcessSettings.bOverride_ColorSaturation = true;
    SpectatorCamera->SetupAttachment(GetRootComponent());
}