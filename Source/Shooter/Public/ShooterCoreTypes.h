#pragma once

#include "ShooterCoreTypes.generated.h"



/*
 * ANIMATIONS
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotified, USkeletalMeshComponent *);



/*
 * HEALTH
 */

DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);



/*
 * Pickups
 */

UENUM(BlueprintType)
enum class EAmmoRestoreType : uint8 {
	ClipsOnly,
	BulletsOnly,
	ClipsAndBullets
};




/*
 * WEAPON
 */

class AShooterBaseWeapon;


DECLARE_MULTICAST_DELEGATE(FOnEmptyClip);


USTRUCT(BlueprintType)
struct FWeaponData {

	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AShooterBaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage *ReloadAnimMontage;

};

USTRUCT(BlueprintType)
struct FAmmoData {

	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = 1))
	int32 BulletsInClip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = 0, EditCondition = "!bInfiniteAmmo"))
	int32 Clips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bInfiniteAmmo;

	UPROPERTY(BlueprintReadOnly)
	int32 SpareBullets = 0;

};

USTRUCT(BlueprintType)
struct FWeaponUIData {

	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D *WeaponIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D *CrosshairIcon;

};