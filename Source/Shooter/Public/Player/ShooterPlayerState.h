// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShooterPlayerState.generated.h"


UCLASS()
class SHOOTER_API AShooterPlayerState : public APlayerState
{

	GENERATED_BODY()
	
public:

	void AddKill(
	);

	int32 GetKills(
	) const;

	void ResetKills(
	);

	void AddDeath(
	);

	int32 GetDeaths(
	) const;

	void ResetDeaths(
	);

	void ResetStatistics(
	);

	void LogStatistics(
	) const;

	int32 TeamID;

	FLinearColor TeamColor;

	int32 PlayerSpawnPointNum;

private:

	int32 KillsNum = 0;

	int32 DeathsNum = 0;

};
