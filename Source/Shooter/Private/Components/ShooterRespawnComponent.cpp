// Shooter, All Rights Reserved


#include "Components/ShooterRespawnComponent.h"

#include "ShooterGameModeBase.h"


UShooterRespawnComponent::UShooterRespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UShooterRespawnComponent::RespawnRequest(
	int32 TimeBeforeRespawn
)
{
	UWorld *World = GetWorld();
	if (!World) {
		return;
	}

	RespawnCountdown = TimeBeforeRespawn;

	World->GetTimerManager().SetTimer(RespawnTimerHandle, this, &UShooterRespawnComponent::RespawnTimerUpdate, true, 1.0f);
}

bool UShooterRespawnComponent::IsRespawnInProgress(
) const
{
	return RespawnCountdown != 0;
}

int32 UShooterRespawnComponent::GetTimeUntilRespawn(
) const
{
	return RespawnCountdown;
}

void UShooterRespawnComponent::RespawnTimerUpdate(
)
{
	if (--RespawnCountdown == 0) {
		UWorld *World = GetWorld();
		if (!World) {
			return;
		}

		World->GetTimerManager().ClearTimer(RespawnTimerHandle);

		AShooterGameModeBase *GameMode = World->GetAuthGameMode<AShooterGameModeBase>();
		if (!GameMode) {
			return;
		}

		GameMode->RespawnImmediate(Cast<AController>(GetOwner()));
	}
}
