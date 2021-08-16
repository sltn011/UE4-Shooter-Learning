// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ShooterGameModeBase.h"
#include "ShooterGameModeCS.generated.h"


UCLASS()
class SHOOTER_API AShooterGameModeCS : public AShooterGameModeBase
{

	GENERATED_BODY()
	
public:

	AShooterGameModeCS(
	);

	void ShuffleSpawnPoints(
	);

	void SetTeamSpawnPoints(
		int32 TeamID,
		TArray<int32> const &SpawnPointsIDs
	);

	void SwapTeams(
	);

	int32 GetTeamPlayerCount(
		int32 TeamID,
		bool bAliveOnly
	);

	bool GetTeamScore(
		int32 TeamID,
		int32 &TeamScore
	);

protected:

	virtual void RegisterKill(
		AController *Killer,
		AController *Victim
	) override;

	virtual void OnRoundStart(
	) override;

	virtual void OnRoundEnd(
	) override;

	virtual void InitTeamsInfo(
	) override;

	virtual AActor *FindPlayerStart_Implementation(
		AController *Player,
		FString const &IncomingName
	) override;

	int32 CountTeamPlayers(
		int32 TeamID
	);

	void CountTeamsPlayers(
		int32 &Team1Players,
		int32 &Team2Players
	);

	void ShuffleArray(
		TArray<int> &Array
	);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = 0))
	int32 EndRoundTimeout = 5;

	int32 Team1AlivePlayersNum = 0;
	int32 Team2AlivePlayersNum = 0;

	int32 Team1Score = 0;
	int32 Team2Score = 0;
};
