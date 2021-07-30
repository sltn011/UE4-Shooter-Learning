// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterBaseWeapon.h"
#include "ShooterRifleWeapon.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UShooterWeaponFXComponent;

UCLASS()
class SHOOTER_API AShooterRifleWeapon : public AShooterBaseWeapon
{
	GENERATED_BODY()

public:

	AShooterRifleWeapon(
	);

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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UShooterWeaponFXComponent *WeaponFXComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem *TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FString TraceTargetName = TEXT("TraceTarget");

private:

	void InitMuzzleFX(
	);

	void SetMuzzleFXVisibility(
		bool bIsVisible
	);

	void SpawnTraceFX(
		FVector const &Begin,
		FVector const &End
	);

	UPROPERTY()
	UNiagaraComponent *MuzzleFXComponent;

	FTimerHandle ShootingTimerHandle;

};
