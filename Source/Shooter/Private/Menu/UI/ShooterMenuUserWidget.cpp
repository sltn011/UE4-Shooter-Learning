// Shooter, All Rights Reserved


#include "Menu/UI/ShooterMenuUserWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

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
    UGameplayStatics::OpenLevel(GetWorld(), TestLevelName);
}

void UShooterMenuUserWidget::OnDustIILevelStart(
)
{
    UGameplayStatics::OpenLevel(GetWorld(), DustIILevelName);
}
