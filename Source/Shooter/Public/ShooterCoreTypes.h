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
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, float);



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
 * VFX
 */

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FDecalData {

	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialInterface *Material;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector Size = FVector(10.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float LifeTime = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float FadeOutTime = 1.0f;
};

USTRUCT(BlueprintType)
struct FImpactData {

	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UNiagaraSystem *Effect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FDecalData ImpactDecal;

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
	int32 SpareAmmo = 0;

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