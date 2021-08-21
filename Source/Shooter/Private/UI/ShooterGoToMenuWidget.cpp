// Shooter, All Rights Reserved


#include "UI/ShooterGoToMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterGoToMenuWidget, All, All);

bool UShooterGoToMenuWidget::Initialize(
)
{
    bool ParentVal = Super::Initialize();

    if (GoToMenuButton) {
        GoToMenuButton->OnClicked.AddDynamic(this, &UShooterGoToMenuWidget::OnGoToMenuButtonClicked);
    }

    return ParentVal;
}

void UShooterGoToMenuWidget::OnGoToMenuButtonClicked(
)
{
    UWorld *World = GetWorld();
    if (!World) {
        return;
    }

    UShooterGameInstance *GameInstance = World->GetGameInstance<UShooterGameInstance>();
    if (!GameInstance) {
        return;
    }

    FName MenuLevelName = GameInstance->GetMenuLevelName();
    if (MenuLevelName.IsNone()) {
        UE_LOG(LogShooterGoToMenuWidget, Error, TEXT("Menu level name not set in GameInstance! Couldn't open Menu!"));
        return;
    }

    UGameplayStatics::OpenLevel(GetWorld(), MenuLevelName);
}
