// Shooter, All Rights Reserved


#include "UI/ShooterGameHUD.h"

#include "Engine/Canvas.h"

void AShooterGameHUD::DrawHUD(
)
{
    Super::DrawHUD();

    DrawCrosshair();
}

void AShooterGameHUD::DrawCrosshair(
)
{
    float ScreenSizeX = Canvas->SizeX;
    float ScreenSizeY = Canvas->SizeY;
    float ScreenCenterX = ScreenSizeX / 2.0f;
    float ScreenCenterY = ScreenSizeY / 2.0f;

    float ArmLength = Size / 2.0f;

    DrawLine(ScreenCenterX - ArmLength, ScreenCenterY, ScreenCenterX + ArmLength, ScreenCenterY, Color, Thickness); // Horizontal
    DrawLine(ScreenCenterX, ScreenCenterY - ArmLength, ScreenCenterX, ScreenCenterY + ArmLength, Color, Thickness); // Vertical
}
