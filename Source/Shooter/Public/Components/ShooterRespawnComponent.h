// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterRespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UShooterRespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UShooterRespawnComponent(
	);

	void RespawnRequest(
		int32 TimeBeforeRespawn
	);

	bool IsRespawnInProgress(
	) const;

	int32 GetTimeUntilRespawn(
	) const;

private:

	void RespawnTimerUpdate(
	);

	FTimerHandle RespawnTimerHandle;

	int32 RespawnCountdown = 0;
		
};
