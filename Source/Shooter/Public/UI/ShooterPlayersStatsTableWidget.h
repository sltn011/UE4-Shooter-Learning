// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCoreTypes.h"
#include "ShooterPlayersStatsTableWidget.generated.h"

class UShooterPlayerStatRowWidget;
class UVerticalBox;

UCLASS()
class SHOOTER_API UShooterPlayersStatsTableWidget : public UUserWidget
{

	GENERATED_BODY()
	
public:

	virtual bool Initialize(
	) override;

protected:

	UFUNCTION()
	void OnGameStateChange(
		EShooterGameState NewState
	);

	UFUNCTION()
	void OnGameUIOverlaySet(
		EShooterGameUIOverlay UIOverlay
	);

	void CreateStatsTable(
	);

	virtual bool ShouldBeCreated(
	);

	UPROPERTY(meta = (BindWidget))
	UVerticalBox *PlayerStatsBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UShooterPlayerStatRowWidget> PlayerStatRowWidgetClass;

private:

	bool bIsCreated = false;

};
