// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickups/ShooterBasePickup.h"
#include "ShooterHealthPickup.generated.h"


UCLASS()
class SHOOTER_API AShooterHealthPickup : public AShooterBasePickup
{

	GENERATED_BODY()

protected:

	virtual void BeginPlay(
	) override;

	virtual bool IsPickableCondition(
		AActor *PickerActor
	) override;

	virtual void PickupEffect(
		APawn *PickerPawn
	) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing", meta = (ClampMin = 0.0f))
	float HealthRestoredAmount = 0.0f;
	
};
