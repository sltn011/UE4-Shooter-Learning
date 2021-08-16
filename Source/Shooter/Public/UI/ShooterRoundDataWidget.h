// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterRoundDataWidget.generated.h"

class AShooterGameModeBase;

UCLASS()
class SHOOTER_API UShooterRoundDataWidget : public UUserWidget
{

	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetTeamPlayerCount(
		bool bAliveOnly
	);

	UFUNCTION(BlueprintCallable, Category = "UI")
	FLinearColor GetTeamColor(
	);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	int32 TeamID = 0;

protected:

	AShooterGameModeBase *GetGameMode(
	) const;

};
