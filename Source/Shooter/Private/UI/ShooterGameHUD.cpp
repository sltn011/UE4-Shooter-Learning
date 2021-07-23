// Shooter, All Rights Reserved


#include "UI/ShooterGameHUD.h"

#include "Blueprint/UserWidget.h"

void AShooterGameHUD::DrawHUD(
)
{
    Super::DrawHUD();
}

void AShooterGameHUD::BeginPlay(
)
{
    Super::BeginPlay();

    UWorld *World = GetWorld();
    check(World);

    if (PlayerHUDWidgetClass) {
        UUserWidget *PlayerHUDWidget = CreateWidget(World, PlayerHUDWidgetClass);
        if (PlayerHUDWidget) {
            PlayerHUDWidget->AddToViewport();
        }
    }
}
