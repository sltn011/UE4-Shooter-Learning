// Shooter, All Rights Reserved


#include "Weapon/ShooterRifleWeapon.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"

void AShooterRifleWeapon::StartShooting(
)
{
	if (!GetWorld()) {
		return;
	}
	GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &AShooterRifleWeapon::MakeShot, 60.0f / ShotsPerMinute, true, 0.0f);
}

void AShooterRifleWeapon::StopShooting(
)
{
	if (!GetWorld()) {
		return;
	}
	GetWorldTimerManager().ClearTimer(ShootingTimerHandle);
}

void AShooterRifleWeapon::MakeShot(
)
{
	UWorld *World = GetWorld();
	if (!World || !WeaponMesh) {
		return;
	}

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) {
		return;
	}

	FHitResult HitResult;
	if (!TraceShot(HitResult, TraceStart, TraceEnd)) {
		return;
	}


	if (HitResult.bBlockingHit) {
		FVector const HitDirection = (HitResult.ImpactPoint - TraceStart).GetSafeNormal();
		FVector const MuzzleForward = GetMuzzleWorldDirection();
		if (FVector::DotProduct(HitDirection, MuzzleForward) < 0.0f) { // Can't shoot backwards
			return;
		}

		DealDamage(HitResult);

		DrawDebugLine(World, TraceStart, HitResult.ImpactPoint, FColor::Red, false, 3.0f);
		DrawDebugSphere(World, HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 3.0f);
	}
	else {
		DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 3.0f);
	}
}

bool AShooterRifleWeapon::GetTraceData(
	FVector &TraceStart,
	FVector &TraceEnd
) const
{
	UWorld *World = GetWorld();
	if (!World) {
		return false;
	}

	FVector CameraLocation;
	FRotator CameraRotation;
	if (!GetCameraViewPoint(CameraLocation, CameraRotation)) {
		return false;
	}

	// Trace data for line from center of camera
	FVector const CameraShotDirection = CameraRotation.Vector();
	FVector const CameraTraceStart = CameraLocation;
	FVector const CameraTraceEnd = CameraTraceStart + CameraShotDirection * BulletMaxDistance;

	FHitResult CameraHitResult;
	if (!TraceShot(CameraHitResult, CameraTraceStart, CameraTraceEnd)) {
		return false;
	}

	FVector const CameraShotEndPoint = CameraHitResult.bBlockingHit ? CameraHitResult.ImpactPoint : CameraTraceEnd;

	// Trace data for line from muzzle
	FVector const MuzzleLocation = GetMuzzleWorldLocation();
	FVector const MuzzleDirection = (CameraShotEndPoint - MuzzleLocation).GetSafeNormal();
	float const SpreadRadians = FMath::DegreesToRadians(BulletSpreadDegrees * 0.5f);
	FVector const MuzzleShotDirection = FMath::VRandCone(MuzzleDirection, SpreadRadians);
	FVector const MuzzleShotEnd = MuzzleLocation + MuzzleShotDirection * BulletMaxDistance;

	TraceStart = MuzzleLocation;
	TraceEnd = MuzzleShotEnd;
	return true;
}

bool AShooterRifleWeapon::DealDamage(
	FHitResult const &HitResult
)
{
	AActor *HitActor = HitResult.GetActor();
	if (!HitResult.bBlockingHit || !HitActor) {
		return false;
	}

	HitActor->TakeDamage(DamagePerShot, {}, GetPlayerController(), this);
	return true;
}