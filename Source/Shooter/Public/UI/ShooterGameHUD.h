// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterCoreTypes.h"
#include "ShooterGameHUD.generated.h"

UCLASS()
class SHOOTER_API AShooterGameHUD : public AHUD
{

	GENERATED_BODY()
	
public:

	virtual void DrawHUD(
	) override;

	EShooterGameUIOverlay GetUIOverlayWidget(
	) const;

	void SetUIOverlayWidget(
		EShooterGameUIOverlay UIOverlay
	);

	FOnGameUIOverlaySet OnGameUIOverlaySet;

protected:

	virtual void BeginPlay(
	) override;

	void CreateWidgets(
	);

	UFUNCTION()
	void OnGameStateChange(
		EShooterGameState NewState
	);

	bool GetCurrentGameState(
		EShooterGameState &GameState
	) const;

	bool CanUIOverlayBeSet(
		EShooterGameUIOverlay UIOverlay
	);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> StatsTableWidgetClass;

	UPROPERTY()
	TMap<EShooterGameState, UUserWidget *> StateWidgetsMap;

	UPROPERTY()
	TMap<EShooterGameUIOverlay, UUserWidget *> UIOverlaysWidgetsMap;

	UUserWidget *CurrentWidget = nullptr;

	EShooterGameUIOverlay CurrentUIOverlay = EShooterGameUIOverlay::NONE;

};
