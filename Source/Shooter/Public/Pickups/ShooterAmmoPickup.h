// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickups/ShooterBasePickup.h"
#include "ShooterCoreTypes.h"
#include "ShooterAmmoPickup.generated.h"

class AShooterBaseWeapon;

UCLASS()
class SHOOTER_API AShooterAmmoPickup : public AShooterBasePickup
{

	GENERATED_BODY()

protected:

	virtual void BeginPlay(
	) override;

	virtual bool IsPickableCondition(
		AActor *PickerActor
	) override;

	virtual void PickupEffect(
		APawn *PickerPawn
	) override;

#if WITH_EDITOR
	virtual bool CanEditChange(
		FProperty const *InProperty
	) const override;
#endif

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo Restore")
	TSubclassOf<AShooterBaseWeapon> RestockedWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo Restore")
	EAmmoRestoreType AmmoRestoreType = EAmmoRestoreType::ClipsOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo Restore", meta = (ClampMin = 0))
	int32 ClipsRestored = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo Restore", meta = (ClampMin = 0))
	int32 BulletsRestored = 0;

};
