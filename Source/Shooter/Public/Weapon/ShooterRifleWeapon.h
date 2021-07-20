// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterBaseWeapon.h"
#include "ShooterRifleWeapon.generated.h"


UCLASS()
class SHOOTER_API AShooterRifleWeapon : public AShooterBaseWeapon
{
	GENERATED_BODY()
	
public:

	virtual void StartShooting(
	) override;

	virtual void StopShooting(
	) override;

protected:

	virtual void BeginPlay(
	) override;

	virtual void MakeShot(
	) override;

	virtual bool GetTraceData(
		FVector &TraceStart,
		FVector &TraceEnd
	) const override;

	bool DealDamage(
		FHitResult const &HitResult
	);

	UPROPERTY(EditDefaultsOnly, Category = "Shooting", meta = (ClampMin = 0.0f))
	float DamagePerShot = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting", meta = (ClampMin = 0.0f))
	float ShotsPerMinute = 600.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting", meta = (ClampMin = 0.0f))
	float BulletSpreadDegrees = 2.5f;

private:

	FTimerHandle ShootingTimerHandle;

};
