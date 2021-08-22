#pragma once

#include "ShooterCoreTypes.generated.h"



/*
 * ANIMATIONS
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotified, USkeletalMeshComponent *);



/*
 * GAME
 */

USTRUCT(BlueprintType)
struct FGameData {

	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = 1, ClampMax = 100))
	int32 NumberOfPlayers = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = 1, ClampMax = 100))
	int32 NumberOfRounds = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = 1, ClampMax = 999))
	int32 RoundTimeInSeconds = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = 1, ClampMax = 999))
	int32 RespawnTimeInSeconds = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Teams")
	FLinearColor DefaultTeamColor = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Teams")
	TArray<FLinearColor> TeamColors;
};

UENUM(BlueprintType)
enum class EShooterGameState : uint8 {
	WaitingToStart,
	InProgress,
	Paused,
	GameOver
};

UENUM(BlueprintType)
enum class EShooterGameUIOverlay : uint8 {
	NONE,
	PlayersStatsTable
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameStateChange, EShooterGameState);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameUIOverlaySet, EShooterGameUIOverlay);



/*
 * HEALTH
 */

DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, float);



/*
 * LEVELS
 */

USTRUCT(BlueprintType)
struct FLevelData {
	
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Level")
	FName LevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Level")
	FName DisplayedLevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Level")
	UTexture2D *LevelIcon;

};



/*
 * PICKUPS
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