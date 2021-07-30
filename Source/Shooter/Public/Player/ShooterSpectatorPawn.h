// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "ShooterSpectatorPawn.generated.h"

class UCameraComponent;

UCLASS()
class SHOOTER_API AShooterSpectatorPawn : public ASpectatorPawn
{

	GENERATED_BODY()

public:

	AShooterSpectatorPawn(
	);

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	UCameraComponent *SpectatorCamera;
	
};
