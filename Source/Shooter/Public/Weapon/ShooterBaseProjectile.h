// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterBaseProjectile.generated.h"

class UProjectileMovementComponent;
class UShooterWeaponFXComponent;
class USphereComponent;

UCLASS()
class SHOOTER_API AShooterBaseProjectile : public AActor
{
	GENERATED_BODY()

public:

	AShooterBaseProjectile(
	);

	FVector ShotDirection = FVector::ZeroVector;

protected:

	virtual void BeginPlay(
	) override;

	UFUNCTION()
	void OnHit(
		UPrimitiveComponent *HitComponent, 
		AActor *OtherActor,
		UPrimitiveComponent *OtherComp,
		FVector NormalImpulse,
		FHitResult const &Hit
	);

	virtual void AtImpactLocation(
		FVector const &ImpactLocation,
		FHitResult const &Hit
	);

	AController *GetOwnerController(
	) const;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent *CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	UProjectileMovementComponent *MovementComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (ClampMin = 0.0f))
	float TimeToLive = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (ClampMin = 0.0f))
	float DamageAmount = 80.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (ClampMin = 0.0f))
	float AreaRadius = 350.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	bool DoFullDamage = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UShooterWeaponFXComponent *WeaponFXComponent;

};
