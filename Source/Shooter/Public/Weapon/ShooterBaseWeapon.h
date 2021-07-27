// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterCoreTypes.h"
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

	bool IsAmmoEmpty(
	) const;

	bool IsClipEmpty(
	) const;

	bool CanReload(
	) const;

	void Reload(
	);

	void AddAmmo(
		int32 AmmoToAdd
	);

	FWeaponUIData GetUIData(
	) const;

	FAmmoData GetDefaultAmmoData(
	) const;

	FAmmoData GetCurrentAmmoData(
	) const;

	FOnEmptyClip OnEmptyClip;

protected:

	virtual void BeginPlay(
	) override;

	virtual void MakeShot(
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

	virtual bool GetTraceData(
		FVector &TraceStart, 
		FVector &TraceEnd
	) const;

	bool TraceShot(
		FHitResult &HitResult,
		FVector const &TraceStart,
		FVector const &TraceEnd
	) const;

	void DecreaseAmmo(
	);

	void LogAmmo(
	) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent *WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	FAmmoData DefaultAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Socket")
	FName MuzzleSocketName = TEXT("MuzzleSocket");

	UPROPERTY(EditDefaultsOnly, Category = "Shooting", meta = (ClampMin = 0.0f))
	float BulletMaxDistance = 10000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FWeaponUIData UIData;


private:

	FAmmoData CurrentAmmo;

};
