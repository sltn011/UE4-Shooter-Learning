// Shooter, All Rights Reserved


#include "Menu/UI/ShooterLevelSelectorWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/Spacer.h"
#include "Menu/UI/ShooterLevelIconWidget.h"
#include "ShooterGameInstance.h"

void UShooterLevelSelectorWidget::SetSelected(
    int32 LevelIndex
)
{
    UShooterGameInstance *GameInstance = GetGameInstance();
    if (!GameInstance || !GameInstance->Levels.IsValidIndex(LevelIndex)) {
        return;
    }

    CurrentlySelected = LevelIndex;
}

int32 UShooterLevelSelectorWidget::GetSelected(
) const
{
    return CurrentlySelected;
}

void UShooterLevelSelectorWidget::NativeOnInitialized(
)
{
    UShooterGameInstance *GameInstance = GetGameInstance();
    if (!GameInstance) {
        return;
    }

    if (LevelsIcons) {
        TArray<FLevelData> const &Levels = GameInstance->Levels;
        for (int32 i = 0; i < Levels.Num(); ++i) {

            UShooterLevelIconWidget *LevelIcon = Cast<UShooterLevelIconWidget>(CreateWidget(GetWorld(), LevelIconClass));
            if (LevelIcon) {
                LevelIcon->ParentWidget = this;
                LevelIcon->LevelIndex = i;

                LevelsIcons->AddChild(LevelIcon);
            }

            if (i + 1 != Levels.Num()) {
                USpacer *Spacer = NewObject<USpacer>(this, FName{ FString::Printf(TEXT("Spacer_%d"), i) });
                if (Spacer) {
                    Spacer->SetSize(FVector2D{ BetweenIconsSpaceWidth, 0.0f });
                    LevelsIcons->AddChild(Spacer);
                }
            }
            
        }
    }
    
    Super::NativeOnInitialized();
}

UShooterGameInstance *UShooterLevelSelectorWidget::GetGameInstance(
) const
{
    UWorld *World = GetWorld();
    if (!World) {
        return nullptr;
    }

    return World->GetGameInstance<UShooterGameInstance>();
}
