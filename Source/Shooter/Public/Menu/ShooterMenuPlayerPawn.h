// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShooterMenuPlayerPawn.generated.h"

class UCameraComponent;

UCLASS()
class SHOOTER_API AShooterMenuPlayerPawn : public APawn
{
	GENERATED_BODY()

public:

	AShooterMenuPlayerPawn(
	);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	UCameraComponent *CameraComponent;

};
