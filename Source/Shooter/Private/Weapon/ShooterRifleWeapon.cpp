// Shooter, All Rights Reserved


#include "Weapon/ShooterRifleWeapon.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Weapon/Components/ShooterWeaponFXComponent.h"

AShooterRifleWeapon::AShooterRifleWeapon(
)
{
	WeaponFXComponent = CreateDefaultSubobject<UShooterWeaponFXComponent>(TEXT("WeaponFXComponent"));
}

void AShooterRifleWeapon::StartShooting(
)
{
	if (!GetWorld()) {
		return;
	}
	InitMuzzleFX();
	GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &AShooterRifleWeapon::MakeShot, 60.0f / ShotsPerMinute, true, 0.0f);
}

void AShooterRifleWeapon::StopShooting(
)
{
	if (!GetWorld()) {
		return;
	}
	SetMuzzleFXVisibility(false);
	GetWorldTimerManager().ClearTimer(ShootingTimerHandle);
}

void AShooterRifleWeapon::BeginPlay(
)
{
	Super::BeginPlay();

	check(WeaponFXComponent);

	check(DamagePerShot > 0.0f);
	check(ShotsPerMinute > 0.0f);
	check(BulletSpreadDegrees >= 0.0f);
}

void AShooterRifleWeapon::MakeShot(
)
{
	if (IsAmmoEmpty()) {
		StopShooting();
		return;
	}

	UWorld *World = GetWorld();
	if (!World || !WeaponMesh || !WeaponFXComponent) {
		StopShooting();
		return;
	}

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) {
		StopShooting();
		return;
	}

	FHitResult HitResult;
	if (!TraceShot(HitResult, TraceStart, TraceEnd)) {
		StopShooting();
		return;
	}

	if (HitResult.bBlockingHit) {
		FVector const HitDirection = (HitResult.ImpactPoint - TraceStart).GetSafeNormal();
		FVector const MuzzleForward = GetMuzzleWorldDirection();
		if (FVector::DotProduct(HitDirection, MuzzleForward) < 0.0f) { // Can't shoot backwards
			return;
		}

		DealDamage(HitResult);

		WeaponFXComponent->PlayImpactFX(HitResult);
	}

	FVector TraceFXBegin = TraceStart;
	FVector TraceFXEnd = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;

	SpawnTraceFX(TraceFXBegin, TraceFXEnd);

	DecreaseAmmo();
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

void AShooterRifleWeapon::InitMuzzleFX(
)
{
	if (!MuzzleFXComponent) {
		MuzzleFXComponent = SpawnMuzzleFX();
	}

	SetMuzzleFXVisibility(true);
}

void AShooterRifleWeapon::SetMuzzleFXVisibility(
	bool bIsVisible
)
{
	if (!MuzzleFXComponent) {
		return;
	}

	MuzzleFXComponent->SetPaused(!bIsVisible);
	MuzzleFXComponent->SetVisibility(bIsVisible);
}

void AShooterRifleWeapon::SpawnTraceFX(
	FVector const &Begin,
	FVector const &End
)
{
	UWorld *World = GetWorld();
	if (!World) {
		return;
	}

	UNiagaraComponent *TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		World,
		TraceFX,
		Begin
	);

	if (TraceFXComponent) {
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, End);
	}
}
