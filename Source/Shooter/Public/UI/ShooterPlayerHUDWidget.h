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

	virtual bool Initialize(
	) override;

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

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnTakeDamage( // No definition needed
	);

protected:

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation *OnDamaged;

private:

	void OnHealthChanged(
		float NewHealth,
		float HealthDelta
	);

	UFUNCTION()
	void OnNewPawn(
		APawn *NewPawn
	);

};
