// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterCoreTypes.h"
#include "ShooterWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UShooterWeaponComponent : public UActorComponent
{

	GENERATED_BODY()

public:	

	UShooterWeaponComponent(
	);

	virtual void StartShooting(
	);

	void StopShooting(
	);

	bool IsShooting(
	) const;

	virtual void EquipNextWeapon(
	);

	void ReloadWeapon(
	);

	bool GetCurrentWeaponUIData(
		FWeaponUIData &UIData
	) const;

	bool GetCurrentWeaponAmmoData(
		FAmmoData &AmmoData
	) const;

	int32 AddAmmoToWeapon(
		EAmmoRestoreType AmmoRestoreType,
		TSubclassOf<AShooterBaseWeapon> RestockedWeaponClass,
		int32 ClipsAdded,
		int32 BulletsAdded
	);

protected:

	virtual void BeginPlay(
	) override;

	virtual void EndPlay(
		EEndPlayReason::Type const EndPlayReason
	) override;

	AShooterBaseWeapon *GetWeaponByClass(
		TSubclassOf<AShooterBaseWeapon> WeaponClass
	) const;

	bool CanShoot(
	) const;

	bool IsClipEmpty(
	) const;

	bool IsAmmoEmpty(
	) const;

	bool CanEquip(
	) const;

	bool CanReload(
	) const;

	void EquipWeapon(
		int32 WeaponIndex
	);

	UPROPERTY()
	TArray<AShooterBaseWeapon *> Weapons;

	UPROPERTY()
	AShooterBaseWeapon *CurrentWeapon = nullptr;

	int32 CurrentWeaponIndex = 0;

	bool bIsShooting = false;

private:

	void SpawnWeapons(
	);

	void Reload(
	);

	void AttachWeaponToSocket(
		AShooterBaseWeapon *Weapon,
		USceneComponent *SceneComponent,
		FName const &SocketName
	);

	USkeletalMeshComponent *GetOwnerSkeletalMesh(
	);

	void InitAnimations(
	);

	void InitEquipAnimation(
	);

	void InitReloadAnimation(
		UAnimMontage *ReloadMontage
	);

	void PlayAnimMontage(
		UAnimMontage *Montage
	);

	int32 CalculateAmmoCanBeAdded(
		AShooterBaseWeapon *RestockedWeapon,
		EAmmoRestoreType AmmoRestoreType,
		int32 ClipsAdded,
		int32 BulletsAdded
	);

	UFUNCTION()
	void OnEquipFinished(
		USkeletalMeshComponent *MeshComp
	);

	UFUNCTION()
	void OnReloadFinished(
		USkeletalMeshComponent *MeshComp
	);

	UFUNCTION()
	void OnEmptyClip(
	);

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquippedSocketName = TEXT("WeaponSocket");

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocketName = TEXT("ArmorySocket");

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage *EquipAnimMontage;

	UPROPERTY()
	UAnimMontage *CurrentReloadAnimMontage = nullptr;

	bool bEquipAnimInProgress = false;
	bool bReloadAnimInProgress = false;
};
