// Shooter, All Rights Reserved


#include "UI/ShooterRoundDataWidget.h"

#include "GameModes/ShooterGameModeCS.h"

int32 UShooterRoundDataWidget::GetTeamPlayerCount(
	bool bAliveOnly
)
{
	AShooterGameModeCS *GameMode = Cast<AShooterGameModeCS>(GetGameMode());
	if (!GameMode) {
		return 0;
	}

	return GameMode->GetTeamPlayerCount(TeamID, bAliveOnly);
}

FLinearColor UShooterRoundDataWidget::GetTeamColor(
)
{
	FLinearColor TeamColor = FLinearColor::White;

	AShooterGameModeCS *GameMode = Cast<AShooterGameModeCS>(GetGameMode());
	if (GameMode) {
		GameMode->GetColorFromTeamID(TeamID, TeamColor);
	}
	return TeamColor;
}

AShooterGameModeBase *UShooterRoundDataWidget::GetGameMode(
) const
{
	UWorld *World = GetWorld();
	if (!World) {
		return nullptr;
	}

	return World->GetAuthGameMode<AShooterGameModeBase>();
}
