// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterBaseWeapon.generated.h"

class APlayerController;
class USkeletalMeshComponent;

UCLASS()
class SHOOTER_API AShooterBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	AShooterBaseWeapon(
	);

	virtual void StartShooting(
	);

	virtual void StopShooting(
	);

protected:

	virtual void BeginPlay(
	) override;

	void MakeShot(
	);

	APlayerController *GetPlayerController(
	) const;

	bool GetCameraViewPoint(
		FVector &Location, 
		FRotator &Rotation
	) const;

	FVector GetMuzzleWorldLocation(
	) const;

	FVector GetMuzzleWorldDirection(
	) const;

	bool GetTraceData(
		FVector &TraceStart, 
		FVector &TraceEnd
	) const;

	bool TraceShot(
		FHitResult &HitResult,
		FVector const &TraceStart,
		FVector const &TraceEnd
	) const;

	bool DealDamage(
		FHitResult const &HitResult
	);

	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent *WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Socket")
	FName MuzzleSocketName = TEXT("MuzzleSocket");

	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	float BulletMaxDistance = 1500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting", meta = (ClipMin = 0.0f))
	float DamagePerShot = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting", meta = (ClipMin = 0.0f))
	float ShotsPerMinute = 600.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting", meta = (ClipMin = 0.0f))
	float BulletSpreadDegrees = 2.5f;

private:

	FTimerHandle ShootingTimerHandle;
};
