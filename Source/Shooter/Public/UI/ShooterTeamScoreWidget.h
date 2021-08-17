// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterTeamScoreWidget.generated.h"

class AShooterGameModeBase;

UCLASS()
class SHOOTER_API UShooterTeamScoreWidget : public UUserWidget
{

	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetTeamScore(
		int32 &TeamScore
	) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetTeamColor(
		FLinearColor &TeamColor
	) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	int32 TeamID = 0;

protected:

	AShooterGameModeBase *GetGameMode(
	) const;

};
