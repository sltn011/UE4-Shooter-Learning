// Shooter, All Rights Reserved


#include "ShooterGameInstance.h"

#include "Sound/ShooterSoundFuncLib.h"

void UShooterGameInstance::Init(
)
{
    Super::Init();

    checkf(Levels.Num() > 0, TEXT("At least one game level must be set in GameInstance!"));
}

FName UShooterGameInstance::GetMenuLevelName(
) const
{
    return MenuLevelName;
}

void UShooterGameInstance::ToggleSound(
)
{
    UShooterSoundFuncLib::ToggleSoundClassVolume(MasterSoundClass);
}
