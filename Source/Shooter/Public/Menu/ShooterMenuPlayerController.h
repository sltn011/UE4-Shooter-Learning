// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/ShooterPlayerController.h"
#include "ShooterMenuPlayerController.generated.h"


UCLASS()
class SHOOTER_API AShooterMenuPlayerController : public AShooterPlayerController
{

	GENERATED_BODY()

protected:

	virtual void BeginPlay(
	) override;
	
};
