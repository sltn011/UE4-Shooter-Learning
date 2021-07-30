// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterBaseProjectile.h"
#include "ShooterVFXProjectile.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UPointLightComponent;


UCLASS()
class SHOOTER_API AShooterVFXProjectile : public AShooterBaseProjectile
{

	GENERATED_BODY()

public:

	AShooterVFXProjectile(
	);

protected:

	virtual void BeginPlay(
	) override;

	virtual void AtImpactLocation(
		FVector const &ImpactLocation,
		FHitResult const &Hit
	) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem *ProjectileFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX", meta = (ClampMin = 0.0f))
	float AfterImpactLifeTime = 0.0f; // Time to wait before deleting projectile from world(needed to let smoke trail disappear by itself)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Light")
	UPointLightComponent *LightComponent;

private:

	void InitProjectileFX(
	);

	void OnImpactDeactivate(
	);

	UPROPERTY()
	UNiagaraComponent *ProjectileFXComponent = nullptr;
	
};
