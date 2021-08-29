// Shooter, All Rights Reserved


#include "Weapon/ShooterRifleWeapon.h"

#include "Components/AudioComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "Weapon/Components/ShooterWeaponFXComponent.h"

AShooterRifleWeapon::AShooterRifleWeapon(
)
{
	WeaponFXComponent = CreateDefaultSubobject<UShooterWeaponFXComponent>(TEXT("WeaponFXComponent"));
}

void AShooterRifleWeapon::StartShooting(
)
{
	if (!GetWorld() || FMath::IsNearlyZero(ShotsPerMinute)) {
		return;
	}

	if (IsAmmoEmpty()) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
		StopShooting();
		return;
	}

	InitFX();
	GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &AShooterRifleWeapon::MakeShot, 60.0f / ShotsPerMinute, true, 0.0f);
}

void AShooterRifleWeapon::StopShooting(
)
{
	if (!GetWorld()) {
		return;
	}

	if (!GetWorldTimerManager().IsTimerActive(ShootingTimerHandle)) {
		return;
	}

	SetFXEnabled(false);
	GetWorldTimerManager().ClearTimer(ShootingTimerHandle);
}

void AShooterRifleWeapon::Zoom(
	bool bEnable
)
{
	Super::Zoom(bEnable);

	APlayerController *Controller = Cast<APlayerController>(GetOwnerController());
	if (!Controller || !Controller->PlayerCameraManager) {
		return;
	}

	Controller->PlayerCameraManager->SetFOV(bEnable ? ZoomedFOV : DefaultFOV);
	CurrentBulletSpread = bEnable ? ZoomedBulletSpreadDegrees : DefaultBulletSpreadDegrees;
}

void AShooterRifleWeapon::OnUnequip(
)
{
	Super::OnUnequip();
	Zoom(false);
}

void AShooterRifleWeapon::BeginPlay(
)
{
	Super::BeginPlay();

	check(WeaponFXComponent);

	check(DamagePerShot >= 0.0f);
	check(ShotsPerMinute > 0.0f);
	check(DefaultBulletSpreadDegrees >= 0.0f);

	CurrentBulletSpread = DefaultBulletSpreadDegrees;

	APlayerController *Controller = Cast<APlayerController>(GetOwnerController());
	if (Controller && Controller->PlayerCameraManager) {
		DefaultFOV = Controller->PlayerCameraManager->GetFOVAngle();
	}
}

void AShooterRifleWeapon::MakeShot(
)
{
	if (IsAmmoEmpty()) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
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
	float const SpreadRadians = FMath::DegreesToRadians(CurrentBulletSpread * 0.5f);
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

	HitActor->TakeDamage(DamagePerShot, {}, GetOwnerController(), this);
	return true;
}

void AShooterRifleWeapon::InitFX(
)
{
	if (!MuzzleFXComponent) {
		MuzzleFXComponent = SpawnMuzzleFX();
	}

	if (!FireAudioComponent) {
		if (Cast<APawn>(GetOwner()) && Cast<APawn>(GetOwner())->GetController()) {
			bool IsPlayer = Cast<APawn>(GetOwner())->GetController()->IsPlayerController();
			FireAudioComponent = UGameplayStatics::SpawnSoundAttached(IsPlayer ? FirePlayerSound : FireBotSound, WeaponMesh, MuzzleSocketName);
		}
	}

	SetFXEnabled(true);
}

void AShooterRifleWeapon::SetFXEnabled(
	bool bIsEnabled
)
{
	if (MuzzleFXComponent) {
		MuzzleFXComponent->SetPaused(!bIsEnabled);
		MuzzleFXComponent->SetVisibility(bIsEnabled);
	}

	if (FireAudioComponent) {
		if (bIsEnabled) {
			FireAudioComponent->Play();
		}
		else {
			FireAudioComponent->FadeOut(0.1f, 0.0f);
			FireAudioComponent = nullptr;
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireStopSound, GetActorLocation(), 1.0f, 1.0f, 0.25f);
		}
	}
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
