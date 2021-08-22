// Shooter, All Rights Reserved


#include "Menu/UI/ShooterLevelIconWidget.h"

#include "Components/Button.h"
#include "Menu/UI/ShooterLevelSelectorWidget.h"
#include "ShooterGameInstance.h"

bool UShooterLevelIconWidget::Initialize(
)
{
    bool ParentVal = Super::Initialize();

    if (SelectButton) {
        SelectButton->OnClicked.AddDynamic(this, &UShooterLevelIconWidget::OnLevelSelected);
    }

    return ParentVal;
}

bool UShooterLevelIconWidget::GetLevelData(
    FLevelData &LevelData
) const
{
    UWorld *World = GetWorld();
    if (!World) {
        return false;
    }

    UShooterGameInstance *GameInstance = World->GetGameInstance<UShooterGameInstance>();
    if (!GameInstance) {
        return false;
    }

    if (!GameInstance->Levels.IsValidIndex(LevelIndex)) {
        return false;
    }

    LevelData = GameInstance->Levels[LevelIndex];
    return true;
}

bool UShooterLevelIconWidget::IsSelected(
) const
{
    if (!ParentWidget) {
        return false;
    }

    return ParentWidget->GetSelected() == LevelIndex;
}

void UShooterLevelIconWidget::OnLevelSelected(
)
{
    if (!ParentWidget) {
        return;
    }

    ParentWidget->SetSelected(LevelIndex);
}
