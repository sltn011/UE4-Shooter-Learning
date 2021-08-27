// Shooter, All Rights Reserved


#include "UI/ShooterBaseWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void UShooterBaseWidget::Show(
)
{
    if (!IsAnimationPlaying(ShowAnimation)) {
        PlayAnimation(ShowAnimation);
    }
    UGameplayStatics::PlaySound2D(GetWorld(), ShowSound);
}