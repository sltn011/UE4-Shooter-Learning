// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterCoreTypes.h"
#include "ShooterPlayerController.generated.h"

class AShooterGameModeBase;
class UShooterRespawnComponent;

UCLASS()
class SHOOTER_API AShooterPlayerController : public APlayerController
{

	GENERATED_BODY()

public:

	AShooterPlayerController(
	);

protected:

	virtual void BeginPlay(
	) override;

	virtual void SetupInputComponent(
	) override;

	void OnTogglePause(
	);

	void OnToggleStatsTable(
	);

	UFUNCTION()
	void OnGameStateChange(
		EShooterGameState NewState
	);

	UFUNCTION()
	void OnGameUIOverlaySet(
		EShooterGameUIOverlay UIOverlay
	);

	bool GetCurrentGameState(
		EShooterGameState &State
	) const;

	AShooterGameModeBase *GetGameMode(
	) const;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Respawn")
	UShooterRespawnComponent *RespawnComponent;
};
