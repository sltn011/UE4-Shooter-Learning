// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterHealthBarWidget.generated.h"

class UProgressBar;

UCLASS()
class SHOOTER_API UShooterHealthBarWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	void SetHealthPercent(
		float HealthPercent
	);

protected:

	UPROPERTY(meta = (BindWidget))
	UProgressBar *HealthBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	float PercentVisibilityThreshold = 0.9f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	float PercentColorThreshold = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FLinearColor HealthyColor = FLinearColor::Green;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FLinearColor InjuredColor = FLinearColor::Red;

};
