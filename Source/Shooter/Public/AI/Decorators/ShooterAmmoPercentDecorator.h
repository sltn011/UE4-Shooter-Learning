// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ShooterAmmoPercentDecorator.generated.h"

class AShooterBaseWeapon;

UCLASS()
class SHOOTER_API UShooterAmmoPercentDecorator : public UBTDecorator
{

	GENERATED_BODY()

public:

	UShooterAmmoPercentDecorator(
	);

protected:

	virtual bool CalculateRawConditionValue(
		UBehaviorTreeComponent &OwnerComp,
		uint8 *NodeMemory
	) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	TSubclassOf<AShooterBaseWeapon> WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float AmmoThreshold = 0.35f;
	
};
