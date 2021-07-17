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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent *WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Socket")
	FName MuzzleSocketName = TEXT("MuzzleSocket");

	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	float BulletMaxDistance = 10000.0f;

};
