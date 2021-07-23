// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCoreTypes.h"
#include "ShooterPlayerHUDWidget.generated.h"


UCLASS()
class SHOOTER_API UShooterPlayerHUDWidget : public UUserWidget
{

	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealthPercent(
	) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerAlive(
	) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsPlayerSpectating(
	) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetCurrentWeaponUIData(
		 FWeaponUIData &UIData
	) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetCurrentWeaponAmmoData(
		FAmmoData &AmmoData
	) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetCurrentWeaponAmmoText(
		FText &AmmoText
	) const;

};
