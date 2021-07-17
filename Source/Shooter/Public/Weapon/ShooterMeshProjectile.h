// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterBaseProjectile.h"
#include "ShooterMeshProjectile.generated.h"

class UStaticMeshComponent;

UCLASS()
class SHOOTER_API AShooterMeshProjectile : public AShooterBaseProjectile
{
	GENERATED_BODY()

public:

	AShooterMeshProjectile(
	);
	
protected:

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	UStaticMeshComponent *ProjectileMesh;

};
