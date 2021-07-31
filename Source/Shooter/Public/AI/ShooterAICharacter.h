// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/ShooterBaseCharacter.h"
#include "ShooterAICharacter.generated.h"


UCLASS()
class SHOOTER_API AShooterAICharacter : public AShooterBaseCharacter
{

	GENERATED_BODY()

public:

	AShooterAICharacter(
		FObjectInitializer const &ObjInit
	);
	
};
