// Shooter, All Rights Reserved


#include "UI/ShooterGameOverMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UShooterGameOverMenuWidget::Initialize(
)
{
    bool ParentVal = Super::Initialize();

    if (RestartLevelButton) {
        RestartLevelButton->OnClicked.AddDynamic(this, &UShooterGameOverMenuWidget::OnRestartLevel);
    }

    return ParentVal;
}

void UShooterGameOverMenuWidget::OnRestartLevel(
)
{
    UWorld *World = GetWorld();
    if (!World) {
        return;
    }

    FString LevelName = World->GetName();
    UGameplayStatics::OpenLevel(World, FName{ LevelName });
}
