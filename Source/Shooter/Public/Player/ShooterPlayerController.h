// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

class UShooterRespawnComponent;

UCLASS()
class SHOOTER_API AShooterPlayerController : public APlayerController
{

	GENERATED_BODY()

public:

	AShooterPlayerController(
	);

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Respawn")
	UShooterRespawnComponent *RespawnComponent;
	
};
