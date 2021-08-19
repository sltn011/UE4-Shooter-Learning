// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShooterCoreTypes.h"
#include "ShooterGameModeBase.generated.h"

class AAIController;

UCLASS()
class SHOOTER_API AShooterGameModeBase : public AGameModeBase
{

	GENERATED_BODY()
	
public:

	AShooterGameModeBase(
	);

	virtual void StartPlay(
	) override;

	virtual UClass *GetDefaultPawnClassForController_Implementation(
		AController *Controller
	) override;

	virtual void RegisterKill(
		AController *Killer,
		AController *Victim
	);

	virtual bool SetPause(
		APlayerController *PlayerController,
		FCanUnpause CanUnpauseDelegate = FCanUnpause()
	) override;

	virtual bool ClearPause(
	) override;

	void LogPlayersStatistics(
	) const;

	int32 GetRoundSecondsRemaining(
	) const;

	int32 GetRoundNumber(
	) const;

	int32 GetNumberOfRounds(
	) const;

	FGameData GetGameData(
	) const;

	bool GetTeamColor(
		int32 TeamID,
		FLinearColor &TeamColor
	);

	void RespawnImmediate(
		AController *Controller
	);

	void RespawnAfterDelay(
		AController *Controller
	);

	bool IsRespawningEnabled(
	) const;

	EShooterGameState GetShooterGameState(
	) const;

	FOnGameStateChange OnGameStateChange;

protected:

	virtual void OnRoundStart(
	);

	virtual void OnRoundEnd(
	);

	virtual void OnGameEnd(
	);

	virtual void InitTeamsInfo(
	);

	void SetPlayerColorFromState(
		AController *Controller
	);

	void SpawnBotsControllers(
	);

	void SetShooterGameState(
		EShooterGameState NewGameState
	);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	FGameData GameData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	TSubclassOf<APawn> AIPawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	bool bRespawningEnabled = true;

	int32 RoundCountdown = 0;

	EShooterGameState ShooterGameState = EShooterGameState::WaitingToStart;

private:

	void UpdateGameTimer(
	);

	void ResetPlayer(
		AController *Controller
	);

	void ResetPlayers(
	);

	void GameOver(
	);

	int32 CurrentRound = 1;

	FTimerHandle GameRoundTimerHandle;

};
