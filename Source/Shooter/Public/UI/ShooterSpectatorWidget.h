// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterSpectatorWidget.generated.h"

class UShooterRespawnComponent;

UCLASS()
class SHOOTER_API UShooterSpectatorWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetTimeUntilRespawn(
		int32 &TimeUntilRespawn
	) const;

private:

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsRespawnInProgress(
	) const;

	UShooterRespawnComponent *GetPlayerRespawnComponent(
	) const;
	
};
