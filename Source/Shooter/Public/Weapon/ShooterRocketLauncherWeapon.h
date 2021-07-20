// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterBaseWeapon.h"
#include "ShooterRocketLauncherWeapon.generated.h"

class AShooterBaseProjectile;

UCLASS()
class SHOOTER_API AShooterRocketLauncherWeapon : public AShooterBaseWeapon
{

	GENERATED_BODY()
	
public:

	virtual void StartShooting(
	) override;

protected:

	virtual void BeginPlay(
	) override;

	virtual void MakeShot(
	) override;

	UPROPERTY(EditDefaultsOnly, Category = "Projectiles")
	TSubclassOf<AShooterBaseProjectile> RocketProjectileClass;

};
