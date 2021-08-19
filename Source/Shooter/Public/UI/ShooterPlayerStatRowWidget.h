// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterPlayerStatRowWidget.generated.h"

class AShooterGameModeBase;
class AShooterPlayerState;

UCLASS()
class SHOOTER_API UShooterPlayerStatRowWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	void SetSpectatedPlayerState(
		AShooterPlayerState *SpectatedState
	);

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetPlayerName(
		FString &PlayerName
	) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetPlayerKillsNum(
		int32 &KillsNum
	) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetPlayerDeathsNum(
		int32 &DeathsNum
	) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetPlayerTeamColor(
		FLinearColor &Color
	) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetTextColor(
		FLinearColor &Color
	) const;

protected:

	AShooterGameModeBase *GetGameMode(
	) const;

	AShooterPlayerState *SpectatedPlayerState = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FLinearColor DefaultPlayerTextColor = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FLinearColor CurrentPlayerTextColor = FLinearColor::Yellow;
	
};
