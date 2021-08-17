// Shooter, All Rights Reserved


#include "UI/ShooterTeamScoreWidget.h"

#include "GameModes/ShooterGameModeCS.h"

bool UShooterTeamScoreWidget::GetTeamScore(
	int32 &TeamScore
) const
{
	AShooterGameModeCS *GameMode = Cast<AShooterGameModeCS>(GetGameMode());
	if (!GameMode) {
		return false;
	}

	return GameMode->GetTeamScore(TeamID, TeamScore);
}

bool UShooterTeamScoreWidget::GetTeamColor(
	FLinearColor &TeamColor
) const
{
	AShooterGameModeCS *GameMode = Cast<AShooterGameModeCS>(GetGameMode());
	if (!GameMode) {
		return false;
	}

	return GameMode->GetTeamColor(TeamID, TeamColor);
}

AShooterGameModeBase *UShooterTeamScoreWidget::GetGameMode(
) const
{
	UWorld *World = GetWorld();
	if (!World) {
		return nullptr;
	}

	return World->GetAuthGameMode<AShooterGameModeBase>();
}
