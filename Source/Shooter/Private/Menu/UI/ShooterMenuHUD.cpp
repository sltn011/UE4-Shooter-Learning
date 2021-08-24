// Shooter, All Rights Reserved


#include "Menu/UI/ShooterMenuHUD.h"

#include "UI/ShooterBaseWidget.h"

void AShooterMenuHUD::BeginPlay(
)
{
    Super::BeginPlay();

    if (MenuWidgetClass) {
        UShooterBaseWidget *MenuWidget = CreateWidget<UShooterBaseWidget>(GetWorld(), MenuWidgetClass);
        if (MenuWidgetClass) {
            MenuWidget->AddToViewport();
            MenuWidget->Show();
        }
    }
}