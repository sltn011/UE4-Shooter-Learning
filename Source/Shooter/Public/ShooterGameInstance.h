// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ShooterCoreTypes.h"
#include "ShooterGameInstance.generated.h"

class USoundClass;

UCLASS()
class SHOOTER_API UShooterGameInstance : public UGameInstance
{

	GENERATED_BODY()

public:

	virtual void Init(
	) override;

	FName GetMenuLevelName(
	) const;

	void ToggleSound(
	);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Levels")
	TArray<FLevelData> Levels;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Levels")
	FName MenuLevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundClass *MasterSoundClass;

};
