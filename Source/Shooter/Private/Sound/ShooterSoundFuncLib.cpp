// Shooter, All Rights Reserved


#include "Sound/ShooterSoundFuncLib.h"

#include "Sound/SoundClass.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterSoundFuncLib, All, All);

void UShooterSoundFuncLib::SetSoundClassVolume(
	USoundClass *SoundClass,
	float Volume
)
{
	if (!SoundClass) {
		return;
	}

	float NewVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
	SoundClass->Properties.Volume = NewVolume;
	UE_LOG(LogShooterSoundFuncLib, Display, TEXT("Set %s volume to %.3f"), *SoundClass->GetName(), NewVolume);
}

void UShooterSoundFuncLib::ToggleSoundClassVolume(
	USoundClass *SoundClass
)
{
	if (!SoundClass) {
		return;
	}

	float NewVolume = FMath::IsNearlyZero(SoundClass->Properties.Volume) ? 1.0f : 0.0f;
	UShooterSoundFuncLib::SetSoundClassVolume(SoundClass, NewVolume);
}