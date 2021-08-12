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

	void RegisterKill(
		AController *Killer,
		AController *Victim
	);

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

	void RespawnImmediate(
		AController *Controller
	);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	FGameData GameData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	TSubclassOf<APawn> AIPawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	bool RespawningEnabled = true;

private:

	void SpawnBots(
	);

	void StartRound(
	);

	void UpdateGameTimer(
	);

	void ResetPlayer(
		AController *Controller
	);

	void ResetPlayers(
	);

	void InitTeamsInfo(
	);

	bool GetColorFromTeamID(
		int32 TeamID,
		FLinearColor &TeamColor
	);

	void SetPlayerColorFromState(
		AController *Controller
	);

	void RespawnAfterDelay(
		AController *Controller
	);

	void GameOver(
	);

	int32 CurrentRound = 1;
	int32 RoundCountdown = 0;

	FTimerHandle GameRoundTimerHandle;

};
