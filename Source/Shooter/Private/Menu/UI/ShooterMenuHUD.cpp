// Shooter, All Rights Reserved


#include "Menu/UI/ShooterMenuHUD.h"

#include "Blueprint/UserWidget.h"

void AShooterMenuHUD::BeginPlay(
)
{
    Super::BeginPlay();

    if (MenuWidgetClass) {
        UUserWidget *MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
        if (MenuWidgetClass) {
            MenuWidget->AddToViewport();
        }
    }
}