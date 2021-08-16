// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterMatchDataWidget.generated.h"


UCLASS()
class SHOOTER_API UShooterMatchDataWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetTeamsScore(
		int32 &Team1Score,
		int32 &Team2Score
	);
	
};
