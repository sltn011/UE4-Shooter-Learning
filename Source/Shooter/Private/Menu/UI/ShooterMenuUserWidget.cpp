// Shooter, All Rights Reserved


#include "Menu/UI/ShooterMenuUserWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterMenuUserWidget, All, All);

bool UShooterMenuUserWidget::Initialize(
)
{
    bool ParentVal = Super::Initialize();

    if (StartTestLevelButton) {
        StartTestLevelButton->OnClicked.AddDynamic(this, &UShooterMenuUserWidget::OnTestLevelStart);
    }

    if (StartDustIILevelButton) {
        StartDustIILevelButton->OnClicked.AddDynamic(this, &UShooterMenuUserWidget::OnDustIILevelStart);
    }

    return ParentVal;
}

void UShooterMenuUserWidget::OnTestLevelStart(
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

    FName TestLevelName = GameInstance->GetTestLevelName();
    if (TestLevelName.IsNone()) {
        UE_LOG(LogShooterMenuUserWidget, Error, TEXT("Test level name not set in GameInstance! Couldn't open TestLevel!"));
        return;
    }

    UGameplayStatics::OpenLevel(GetWorld(), TestLevelName);
}

void UShooterMenuUserWidget::OnDustIILevelStart(
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

    FName DustIILevelName = GameInstance->GetDustIILevelName();
    if (DustIILevelName.IsNone()) {
        UE_LOG(LogShooterMenuUserWidget, Error, TEXT("Dust2 level name not set in GameInstance! Couldn't open Dust2!"));
        return;
    }

    UGameplayStatics::OpenLevel(GetWorld(), DustIILevelName);
}
