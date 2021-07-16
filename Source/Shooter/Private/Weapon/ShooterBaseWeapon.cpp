// Shooter, All Rights Reserved


#include "Weapon/ShooterBaseWeapon.h"

#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterBaseWeapon, All, All);

AShooterBaseWeapon::AShooterBaseWeapon(
)
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
}

void AShooterBaseWeapon::BeginPlay(
)
{
	Super::BeginPlay();
}

void AShooterBaseWeapon::MakeShot(
)
{
	UWorld *World = GetWorld();
	if (!World || !WeaponMesh) {
		return;
	}

	FVector MuzzleTraceStart, MuzzleTraceEnd;
	if (!GetTraceData(MuzzleTraceStart, MuzzleTraceEnd)) { // Can't do tracing if an error occured
		return;
	}

	FHitResult MuzzleHitResult;
	if (!TraceShot(MuzzleHitResult, MuzzleTraceStart, MuzzleTraceEnd)) { // Can't  do tracing if an error occured
		return;
	}


	if (MuzzleHitResult.bBlockingHit) {
		FVector const HitDirection = (MuzzleHitResult.ImpactPoint - MuzzleTraceStart).GetSafeNormal();
		FVector const MuzzleForward = GetMuzzleWorldDirection();
		if (FVector::DotProduct(HitDirection, MuzzleForward) < 0.0f) { // Can't shoot backwards
			return;
		}

		DealDamage(MuzzleHitResult);

		DrawDebugLine(World, MuzzleTraceStart, MuzzleHitResult.ImpactPoint, FColor::Red, false, 3.0f);
		DrawDebugSphere(World, MuzzleHitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 3.0f);
	}
	else {
		DrawDebugLine(World, MuzzleTraceStart, MuzzleTraceEnd, FColor::Red, false, 3.0f);
	}
}

APlayerController *AShooterBaseWeapon::GetPlayerController(
) const
{
	ACharacter const *Player = Cast<ACharacter>(GetOwner());
	if (!Player) {
		return nullptr;
	}

	return Player->GetController<APlayerController>();
}

bool AShooterBaseWeapon::GetCameraViewPoint(
	FVector &Location,
	FRotator &Rotation
) const
{
	APlayerController *Controller = GetPlayerController();
	if (!Controller) {
		return false;
	}

	Controller->GetPlayerViewPoint(Location, Rotation);
	return true;
}

FVector AShooterBaseWeapon::GetMuzzleWorldLocation(
) const
{
	return WeaponMesh ? WeaponMesh->GetSocketLocation(MuzzleSocketName) : FVector::ZeroVector;
}

FVector AShooterBaseWeapon::GetMuzzleWorldDirection(
) const
{
	return WeaponMesh ? WeaponMesh->GetSocketQuaternion(MuzzleSocketName).GetForwardVector() : FVector::ZeroVector;
}

bool AShooterBaseWeapon::GetTraceData(
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

bool AShooterBaseWeapon::TraceShot(
	FHitResult &HitResult,
	FVector const &TraceStart, 
	FVector const &TraceEnd
) const
{
	UWorld *World = GetWorld();
	if (!World) {
		return false;
	}

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams);

	return true;
}

bool AShooterBaseWeapon::DealDamage(
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

void AShooterBaseWeapon::StartShooting(
)
{
	if (!GetWorld()) {
		return;
	}
	GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &AShooterBaseWeapon::MakeShot, 60.0f / ShotsPerMinute, true, 0.0f);
}

void AShooterBaseWeapon::StopShooting(
)
{
	if (!GetWorld()) {
		return;
	}
	GetWorldTimerManager().ClearTimer(ShootingTimerHandle);
}
