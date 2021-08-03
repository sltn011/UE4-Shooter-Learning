// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ShooterWeaponComponent.h"
#include "ShooterAIWeaponComponent.generated.h"


UCLASS()
class SHOOTER_API UShooterAIWeaponComponent : public UShooterWeaponComponent
{

	GENERATED_BODY()
	
public:

	virtual void StartShooting(
	) override;

	virtual void EquipNextWeapon(
	) override;

};
