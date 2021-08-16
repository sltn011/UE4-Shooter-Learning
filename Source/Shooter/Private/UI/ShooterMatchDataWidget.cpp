// Shooter, All Rights Reserved


#include "UI/ShooterMatchDataWidget.h"

#include "GameModes/ShooterGameModeCS.h"

bool UShooterMatchDataWidget::GetTeamsScore(
	int32 &Team1Score,
	int32 &Team2Score
)
{
	UWorld *World = GetWorld();
	if (!World) {
		return false;
	}

	AShooterGameModeCS *GameMode = World->GetAuthGameMode<AShooterGameModeCS>();
	if (!GameMode) {
		return false;
	}

	return GameMode->GetTeamScore(1, Team1Score) && GameMode->GetTeamScore(2, Team2Score);
}