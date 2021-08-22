// Shooter, All Rights Reserved


#include "Menu/UI/ShooterMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterMenuWidget, All, All);

bool UShooterMenuWidget::Initialize(
)
{
    bool ParentVal = Super::Initialize();

    if (StartTestLevelButton) {
        StartTestLevelButton->OnClicked.AddDynamic(this, &UShooterMenuWidget::OnTestLevelStart);
    }

    if (StartDustIILevelButton) {
        StartDustIILevelButton->OnClicked.AddDynamic(this, &UShooterMenuWidget::OnDustIILevelStart);
    }

    if (QuitGameButton) {
        QuitGameButton->OnClicked.AddDynamic(this, &UShooterMenuWidget::OnQuitGame);
    }

    return ParentVal;
}

void UShooterMenuWidget::OnTestLevelStart(
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
        UE_LOG(LogShooterMenuWidget, Error, TEXT("Test level name not set in GameInstance! Couldn't open TestLevel!"));
        return;
    }

    UGameplayStatics::OpenLevel(GetWorld(), TestLevelName);
}

void UShooterMenuWidget::OnDustIILevelStart(
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
        UE_LOG(LogShooterMenuWidget, Error, TEXT("Dust2 level name not set in GameInstance! Couldn't open Dust2!"));
        return;
    }

    UGameplayStatics::OpenLevel(GetWorld(), DustIILevelName);
}

void UShooterMenuWidget::OnQuitGame(
)
{
    UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}
