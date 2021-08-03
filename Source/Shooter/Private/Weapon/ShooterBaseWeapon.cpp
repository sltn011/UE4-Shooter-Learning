// Shooter, All Rights Reserved


#include "Weapon/ShooterBaseWeapon.h"

#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
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

bool AShooterBaseWeapon::IsAmmoEmpty(
) const
{
	// Only true when ammo is not infinite and current clip is empty and no more spare bullets left
	return !CurrentAmmo.bInfiniteAmmo && IsClipEmpty() && CurrentAmmo.SpareBullets == 0;
}

bool AShooterBaseWeapon::IsClipEmpty(
) const
{
	return CurrentAmmo.BulletsInClip == 0;
}

bool AShooterBaseWeapon::CanReload(
) const
{
	return CurrentAmmo.BulletsInClip < DefaultAmmo.BulletsInClip && (CurrentAmmo.SpareBullets > 0 || CurrentAmmo.bInfiniteAmmo);
}

void AShooterBaseWeapon::Reload(
)
{
	if (!CanReload()) {
		UE_LOG(LogShooterBaseWeapon, Display, TEXT("ChangeClip failed - CanReload() is false"));
		return;
	}

	int32 BulletsMissing = DefaultAmmo.BulletsInClip - CurrentAmmo.BulletsInClip;
	int32 BulletsCanBeAdded = CurrentAmmo.bInfiniteAmmo ? BulletsMissing : (BulletsMissing > CurrentAmmo.SpareBullets ? CurrentAmmo.SpareBullets : BulletsMissing);

	if (!CurrentAmmo.bInfiniteAmmo) {
		CurrentAmmo.SpareBullets -= BulletsCanBeAdded;
	}
	CurrentAmmo.BulletsInClip += BulletsCanBeAdded;
}

void AShooterBaseWeapon::AddAmmo(
	int32 AmmoToAdd
)
{
	if (AmmoToAdd <= 0) {
		return;
	}

	CurrentAmmo.SpareBullets += AmmoToAdd;
}

FWeaponUIData AShooterBaseWeapon::GetUIData(
) const
{
	return UIData;
}

FAmmoData AShooterBaseWeapon::GetDefaultAmmoData(
) const
{
	return DefaultAmmo;
}

FAmmoData AShooterBaseWeapon::GetCurrentAmmoData(
) const
{
	return CurrentAmmo;
}

void AShooterBaseWeapon::BeginPlay(
)
{
	Super::BeginPlay();

	check(WeaponMesh);

	check(BulletMaxDistance > 0.0f);

	check(DefaultAmmo.BulletsInClip > 0);
	check(DefaultAmmo.Clips >= 0);

	CurrentAmmo = DefaultAmmo;
	CurrentAmmo.SpareBullets = CurrentAmmo.BulletsInClip * CurrentAmmo.Clips;
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
	ACharacter *ShooterCharacter = Cast<ACharacter>(GetOwner());
	if (!ShooterCharacter) {
		return false;
	}

	if (ShooterCharacter->IsPlayerControlled()) {
		APlayerController *Controller = GetPlayerController();
		if (!Controller) {
			return false;
		}

		Controller->GetPlayerViewPoint(Location, Rotation);
	}
	else {
		Location = GetMuzzleWorldLocation();
		Rotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
	}

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
	CollisionParams.bReturnPhysicalMaterial = true;

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

	--CurrentAmmo.BulletsInClip;
	LogAmmo();

	if (IsClipEmpty() && !IsAmmoEmpty()) {
		OnEmptyClip.Broadcast();
	}
}

void AShooterBaseWeapon::LogAmmo(
) const
{
	FString AmmoInfo = FString::Printf(
		TEXT("Weapon: %s, Bullets: %d, SpareBullets: %s, IsClipEmpty: %d, IsAmmoEmpty: %d"), 
		*GetName(),
		CurrentAmmo.BulletsInClip,
		CurrentAmmo.bInfiniteAmmo ? TEXT("Infinite") : *(FString::FromInt(CurrentAmmo.SpareBullets)),
		IsClipEmpty(),
		IsAmmoEmpty()
	);

	UE_LOG(LogShooterBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

UNiagaraComponent *AShooterBaseWeapon::SpawnMuzzleFX(
)
{
	return UNiagaraFunctionLibrary::SpawnSystemAttached(
		MuzzleFX,
		WeaponMesh,
		MuzzleSocketName,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true
	);
}
