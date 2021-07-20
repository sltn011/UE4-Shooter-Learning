// Shooter, All Rights Reserved


#include "Weapon/ShooterRocketLauncherWeapon.h"

#include "Weapon/ShooterBaseProjectile.h"

void AShooterRocketLauncherWeapon::StartShooting(
)
{
    MakeShot();
}

void AShooterRocketLauncherWeapon::BeginPlay(
)
{
	Super::BeginPlay();

	checkf(RocketProjectileClass, TEXT("RocketProjectileClass is not set!"))
}

void AShooterRocketLauncherWeapon::MakeShot(
)
{
	if (IsAmmoEmpty()) {
		return;
	}

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

	FVector const EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	FVector const ShotDirection = (EndPoint - TraceStart).GetSafeNormal();

    FTransform const SpawnTransform{ FRotator::ZeroRotator, GetMuzzleWorldLocation() };
	AShooterBaseProjectile *Projectile = World->SpawnActorDeferred<AShooterBaseProjectile>(RocketProjectileClass, SpawnTransform);
	if (!Projectile) {
		return;
	}

	Projectile->SetOwner(GetOwner()); // Launcher owner owns its projectiles
	Projectile->ShotDirection = ShotDirection;
	Projectile->FinishSpawning(SpawnTransform);

	DecreaseAmmo();
}
