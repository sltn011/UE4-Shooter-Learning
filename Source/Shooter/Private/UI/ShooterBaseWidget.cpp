// Shooter, All Rights Reserved


#include "UI/ShooterBaseWidget.h"

void UShooterBaseWidget::Show(
)
{
    if (!IsAnimationPlaying(ShowAnimation)) {
        PlayAnimation(ShowAnimation);
    }
}