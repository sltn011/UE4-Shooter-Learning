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
	bool GetRoundsDataText(
		FText &RoundNumText
	) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetRoundTimeRemainigText(
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
