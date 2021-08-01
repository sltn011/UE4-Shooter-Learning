// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "ShooterAIPerceptionComponent.generated.h"


UCLASS()
class SHOOTER_API UShooterAIPerceptionComponent : public UAIPerceptionComponent
{

	GENERATED_BODY()

public:

	AActor *GetClosestVisibleActor(
	) const;
	
};
