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

void AShooterBaseWeapon::StartShooting(
)
{
}

void AShooterBaseWeapon::StopShooting(
)
{
}

bool AShooterBaseWeapon::CanReload(
) const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && (CurrentAmmo.Clips > 0 || CurrentAmmo.bInfiniteAmmo);
}

void AShooterBaseWeapon::ChangeClip(
)
{
	if (!CanReload()) {
		UE_LOG(LogShooterBaseWeapon, Display, TEXT("ChangeClip failed - CanReload() is false"));
		return;
	}

	if (CurrentAmmo.Clips > 0 && !CurrentAmmo.bInfiniteAmmo) {
		--CurrentAmmo.Clips;
	}
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}

void AShooterBaseWeapon::BeginPlay(
)
{
	Super::BeginPlay();

	check(WeaponMesh);

	check(BulletMaxDistance > 0.0f);

	CurrentAmmo = DefaultAmmo;
}

void AShooterBaseWeapon::MakeShot(
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

		DrawDebugLine(World, TraceStart, HitResult.ImpactPoint, FColor::Red, false, 3.0f);
		DrawDebugSphere(World, HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 3.0f);
	}
	else {
		DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 3.0f);
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
	FVector const MuzzleShotDirection = (CameraShotEndPoint - MuzzleLocation).GetSafeNormal();
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

void AShooterBaseWeapon::DecreaseAmmo(
)
{
	if (IsAmmoEmpty()) {
		UE_LOG(LogShooterBaseWeapon, Display, TEXT("DecreaseAmmo failed - IsAmmoEmpty is true"));
		return;
	}

	if (IsClipEmpty()) {
		OnEmptyClip.Broadcast();
		return;
	}

	--CurrentAmmo.Bullets;
	LogAmmo();

	if (IsClipEmpty() && !IsAmmoEmpty()) {
		OnEmptyClip.Broadcast();
	}
}

bool AShooterBaseWeapon::IsAmmoEmpty(
) const
{
	// Only true when ammo is not infinite and current clip is empty and no more clips left
	return !CurrentAmmo.bInfiniteAmmo && IsClipEmpty() && CurrentAmmo.Clips == 0;
}

bool AShooterBaseWeapon::IsClipEmpty(
) const
{
	return CurrentAmmo.Bullets == 0;
}

void AShooterBaseWeapon::LogAmmo(
) const
{
	FString AmmoInfo = FString::Printf(
		TEXT("Weapon: %s, Bullets: %d, Clips: %s, IsClipEmpty: %d, IsAmmoEmpty: %d"), 
		*GetName(),
		CurrentAmmo.Bullets,
		CurrentAmmo.bInfiniteAmmo ? TEXT("Infinite") : *(FString::FromInt(CurrentAmmo.Clips)),
		IsClipEmpty(),
		IsAmmoEmpty()
	);

	UE_LOG(LogShooterBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}
