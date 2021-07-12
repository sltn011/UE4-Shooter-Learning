// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterCharMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UShooterCharMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "1.5", ClampMax = "10.0"))
	float RunningSpeedMultiplier = 2.0f;

public:

	virtual float GetMaxSpeed(
	) const override;
	
};
