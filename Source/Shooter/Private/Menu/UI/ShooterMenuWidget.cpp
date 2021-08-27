// Shooter, All Rights Reserved


#include "Menu/UI/ShooterMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Menu/UI/ShooterLevelSelectorWidget.h"
#include "ShooterGameInstance.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterMenuWidget, All, All);

bool UShooterMenuWidget::Initialize(
)
{
    bool ParentVal = Super::Initialize();

    if (PlaySelectedLevelButton) {
        PlaySelectedLevelButton->OnClicked.AddDynamic(this, &UShooterMenuWidget::OnPlaySelectedLevel);
    }

    if (QuitGameButton) {
        QuitGameButton->OnClicked.AddDynamic(this, &UShooterMenuWidget::OnQuitGame);
    }

    return ParentVal;
}

void UShooterMenuWidget::OnAnimationFinished_Implementation(
    UWidgetAnimation const *Animation
)
{
    if (Animation != LoadingAnimation) {
        return;
    }

    LoadAndPlaySelectedLevel();
}

void UShooterMenuWidget::LoadAndPlaySelectedLevel(
)
{
    if (!LevelSelector) {
        return;
    }

    UWorld *World = GetWorld();
    if (!World) {
        return;
    }

    UShooterGameInstance *GameInstance = World->GetGameInstance<UShooterGameInstance>();
    if (!GameInstance) {
        return;
    }

    int32 SelectedLevelIndex = LevelSelector->GetSelected();
    if (!GameInstance->Levels.IsValidIndex(SelectedLevelIndex)) {
        return;
    }

    FName SelectedLevelName = GameInstance->Levels[SelectedLevelIndex].LevelName;
    if (SelectedLevelName.IsNone()) {
        UE_LOG(LogShooterMenuWidget, Error, TEXT("Level Name is not properly set in Game Instance for selected level!"));
        return;
    }

    UGameplayStatics::OpenLevel(World, SelectedLevelName);
}

void UShooterMenuWidget::OnPlaySelectedLevel(
)
{
    if (!IsAnimationPlaying(LoadingAnimation)) {
        PlayAnimation(LoadingAnimation);
    }
    UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
}

void UShooterMenuWidget::OnQuitGame(
)
{
    UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}
