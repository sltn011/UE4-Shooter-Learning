// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ShooterSoundFuncLib.generated.h"

class USoundClass;

UCLASS()
class SHOOTER_API UShooterSoundFuncLib : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static void SetSoundClassVolume(
		USoundClass *SoundClass,
		float Volume
	);

	UFUNCTION(BlueprintCallable)
	static void ToggleSoundClassVolume(
		USoundClass *SoundClass
	);
	
};
