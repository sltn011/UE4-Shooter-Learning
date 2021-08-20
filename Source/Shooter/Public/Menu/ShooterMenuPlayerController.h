// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterMenuPlayerController.generated.h"


UCLASS()
class SHOOTER_API AShooterMenuPlayerController : public APlayerController
{

	GENERATED_BODY()

protected:

	virtual void BeginPlay(
	) override;
	
};
