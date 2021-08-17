// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterGameDataWidget.generated.h"

class AShooterGameModeBase;

UCLASS()
class SHOOTER_API UShooterGameDataWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetRoundsData(
		int32 &CurrentRoundsNum,
		int32 &MaxRoundsNum
	) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetRoundTimeRemaining(
		int32 &MinutesRemaining,
		int32 &SecondsRemaining
	) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetRoundTimeRemainingText(
		FText &TimeRemainingText
	) const;
	
protected:

	AShooterGameModeBase *GetGameMode(
	) const;

	bool GetCurrentRoundNum(
		int32 &RoundNum
	) const;

	bool GetMaxRoundsNum(
		int32 &MaxRoundsNum
	) const;

	bool GetRoundSecondsRemaining(
		int32 &SecondsRemaining
	) const;
};
