// Shooter, All Rights Reserved


#include "Components/ShooterWeaponComponent.h"

#include "Animations/AnimUtils.h"
#include "Animations/ShooterEquipFinishedAnimNotify.h"
#include "Animations/ShooterReloadFinishedAnimNotify.h"
#include "Player/ShooterBaseCharacter.h"
#include "Weapon/ShooterBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterWeaponComponent, All, All);

static constexpr int32 NUM_WEAPONS_REQUIRED = 2;

UShooterWeaponComponent::UShooterWeaponComponent(
)
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UShooterWeaponComponent::StartShooting(
)
{
	if (!CanShoot()) {
		return;
	}
	bIsShooting = true;
	CurrentWeapon->StartShooting();
}

void UShooterWeaponComponent::StopShooting(
)
{
	if (!CurrentWeapon) {
		return;
	}
	bIsShooting = false;
	CurrentWeapon->StopShooting();
}

bool UShooterWeaponComponent::IsShooting(
) const
{
	return bIsShooting;
}

void UShooterWeaponComponent::EquipNextWeapon(
)
{
	if (!CanEquip()) {
		return;
	}

	int32 NextWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(NextWeaponIndex);
}

void UShooterWeaponComponent::ReloadWeapon(
)
{
	if (!CanReload()) {
		return;
	}
	Reload();
}

bool UShooterWeaponComponent::GetCurrentWeaponUIData(
	FWeaponUIData &UIData
) const
{
	if (!CurrentWeapon) {
		return false;
	}

	UIData = CurrentWeapon->GetUIData();
	return true;
}

bool UShooterWeaponComponent::GetCurrentWeaponAmmoData(
	FAmmoData &AmmoData
) const
{
	if (!CurrentWeapon) {
		return false;
	}

	AmmoData = CurrentWeapon->GetCurrentAmmoData();
	return true;
}

int32 UShooterWeaponComponent::AddAmmoToWeapon(
	EAmmoRestoreType AmmoRestoreType,
	TSubclassOf<AShooterBaseWeapon> RestockedWeaponClass,
	int32 ClipsAdded,
	int32 BulletsAdded
)
{
	AShooterBaseWeapon *RestockedWeapon = GetWeaponByClass(RestockedWeaponClass);
	if (!RestockedWeapon) {
		return 0;
	}

	int32 AmmoToAdd = CalculateAmmoCanBeAdded(RestockedWeapon, AmmoRestoreType, ClipsAdded, BulletsAdded);

	bool bClipWasEmpty = RestockedWeapon->IsClipEmpty();
	RestockedWeapon->AddAmmo(AmmoToAdd);

	if (CurrentWeapon == RestockedWeapon && bClipWasEmpty && RestockedWeapon->CanReload()) {
		Reload();
	}

	return AmmoToAdd;
}

void UShooterWeaponComponent::BeginPlay(
)
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == NUM_WEAPONS_REQUIRED, TEXT("Character must have %d weapons! You have: %d"), NUM_WEAPONS_REQUIRED, WeaponData.Num());

	checkf(EquipAnimMontage, TEXT("EquipAnimMontage is not set!"));

	for (auto const &SingleWeaponData : WeaponData) {
		checkf(SingleWeaponData.WeaponClass, TEXT("WeaponClass is not set for all weapons!"));
		checkf(SingleWeaponData.ReloadAnimMontage, TEXT("ReloadAnimMontage is not set for all weapons!"));
	}

	InitAnimations();

	CurrentWeaponIndex = 0;
	SpawnWeapons();

	EquipWeapon(CurrentWeaponIndex);
}

void UShooterWeaponComponent::EndPlay(
	EEndPlayReason::Type const EndPlayReason
)
{
	StopShooting();

	CurrentWeapon = nullptr;

	for (AShooterBaseWeapon *Weapon : Weapons) {
		if (Weapon) {
			Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			Weapon->Destroy();
		}
	}
	Weapons.Empty();

	Super::EndPlay(EndPlayReason);
}

AShooterBaseWeapon *UShooterWeaponComponent::GetWeaponByClass(
	TSubclassOf<AShooterBaseWeapon> WeaponClass
) const
{
	for (AShooterBaseWeapon *Weapon : Weapons) {
		if (Weapon->IsA(WeaponClass)) {
			return Weapon;
		}
	}
	return nullptr;
}

void UShooterWeaponComponent::SpawnWeapons(
)
{
	UWorld *World = GetWorld();
	if (!World) {
		return;
	}

	ACharacter *Character = Cast<ACharacter>(GetOwner());
	if (!Character) {
		return;
	}

	USkeletalMeshComponent *SkeletalMesh = GetOwnerSkeletalMesh();
	if (!SkeletalMesh) {
		return;
	}

	for (auto &SingleWeaponData : WeaponData) {
		AShooterBaseWeapon *Weapon = World->SpawnActor<AShooterBaseWeapon>(SingleWeaponData.WeaponClass);
		if (!Weapon) {
			UE_LOG(LogShooterWeaponComponent, Warning, TEXT("Couldn't spawn weapon %s for %s"), *SingleWeaponData.WeaponClass->GetName(), *Character->GetName());
			continue;
		}
		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);
		AttachWeaponToSocket(Weapon, SkeletalMesh, WeaponArmorySocketName);
		Weapon->OnEmptyClip.AddUObject(this, &UShooterWeaponComponent::OnEmptyClip);
	}

}

void UShooterWeaponComponent::AttachWeaponToSocket(
	AShooterBaseWeapon *Weapon,
	USceneComponent *SceneComponent,
	FName const &SocketName
)
{
	if (!Weapon || !SceneComponent) {
		return;
	}
	FAttachmentTransformRules AttachmentRules{ EAttachmentRule::SnapToTarget, false };
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UShooterWeaponComponent::EquipWeapon(
	int32 WeaponIndex
)
{
	if (!Weapons.IsValidIndex(WeaponIndex)) {
		return;
	}

	USkeletalMeshComponent *SkeletalMesh = GetOwnerSkeletalMesh();
	if (!SkeletalMesh) {
		return;
	}

	// Find selected weapon and it's reload animation
	AShooterBaseWeapon *Weapon = Weapons[WeaponIndex];
	FWeaponData *SelectedWeaponData = nullptr;
	if (Weapon) {
		SelectedWeaponData = WeaponData.FindByPredicate(
			[&Weapon](FWeaponData const &Data) {
				return Data.WeaponClass == Weapon->GetClass();
			}
		);
	}
	UAnimMontage *WeaponReloadMontage = SelectedWeaponData ? SelectedWeaponData->ReloadAnimMontage : nullptr;

	StopShooting();

	if (CurrentWeapon != Weapon) { // If equipping not current weapon - put old one on the back
		AttachWeaponToSocket(CurrentWeapon, SkeletalMesh, WeaponArmorySocketName); // Place current weapon on the back
	}

	AttachWeaponToSocket(Weapon, SkeletalMesh, WeaponEquippedSocketName); // Equip selected weapon

	CurrentWeapon = Weapon;
	CurrentReloadAnimMontage = WeaponReloadMontage;
	CurrentWeaponIndex = WeaponIndex;

	bReloadAnimInProgress = false; // If reload animation was playing it will be cancelled - reset it's flag
	bEquipAnimInProgress = true;
	PlayAnimMontage(EquipAnimMontage);
}

void UShooterWeaponComponent::PlayAnimMontage(
	UAnimMontage *Montage
)
{
	ACharacter *Character = Cast<ACharacter>(GetOwner());
	if (!Character) {
		return;
	}

	Character->PlayAnimMontage(Montage);
}

USkeletalMeshComponent *UShooterWeaponComponent::GetOwnerSkeletalMesh(
)
{
	ACharacter *Character = Cast<ACharacter>(GetOwner());
	if (!Character) {
		return nullptr;
	}
	return Character->GetMesh();
}

void UShooterWeaponComponent::InitAnimations(
)
{
	InitEquipAnimation();

	for (FWeaponData &SingleWeaponData : WeaponData) {
		InitReloadAnimation(SingleWeaponData.ReloadAnimMontage);
	}
}

void UShooterWeaponComponent::InitEquipAnimation(
)
{
	UShooterEquipFinishedAnimNotify *EquipFinishedNotify = AnimUtils::FindNotifyByClass<UShooterEquipFinishedAnimNotify>(EquipAnimMontage);
	if (EquipFinishedNotify) {
		EquipFinishedNotify->OnNotified.AddUObject(this, &UShooterWeaponComponent::OnEquipFinished);
	}
	else {
		UE_LOG(LogShooterWeaponComponent, Error, TEXT("EquipFinishedAnimNotify not set for EquipAnimMontage!"));
		checkNoEntry();
	}
}

void UShooterWeaponComponent::InitReloadAnimation(
	UAnimMontage *ReloadMontage
)
{
	UShooterReloadFinishedAnimNotify *ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UShooterReloadFinishedAnimNotify>(ReloadMontage);
	if (ReloadFinishedNotify) {
		ReloadFinishedNotify->OnNotified.AddUObject(this, &UShooterWeaponComponent::OnReloadFinished);
	}
	else {
		UE_LOG(LogShooterWeaponComponent, Error, TEXT("ReloadFinishedAnimNotify not set for one of ReloadMontages!"));
		checkNoEntry();
	}
}

bool UShooterWeaponComponent::CanShoot(
) const
{
	return CurrentWeapon && !CurrentWeapon->IsClipEmpty() && !bEquipAnimInProgress && !bReloadAnimInProgress;
}

bool UShooterWeaponComponent::CanEquip(
) const
{
	return Weapons.Num() && !bEquipAnimInProgress; // Can change weapon even if reloading - this will cancel reload action
}

bool UShooterWeaponComponent::CanReload(
) const
{
	return CurrentWeapon && CurrentWeapon->CanReload() && !bEquipAnimInProgress && !bReloadAnimInProgress;
}

void UShooterWeaponComponent::Reload(
)
{
	if (!CanReload()) {
		return;
	}

	CurrentWeapon->StopShooting();
	//CurrentWeapon->Reload(); - Updating ammo data is done only after animation is finished

	bReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}

int32 UShooterWeaponComponent::CalculateAmmoCanBeAdded(
	AShooterBaseWeapon *RestockedWeapon,
	EAmmoRestoreType AmmoRestoreType,
	int32 ClipsAdded,
	int32 BulletsAdded
)
{
	if (!RestockedWeapon) {
		return 0;
	}

	FAmmoData DefaultAmmoData = RestockedWeapon->GetDefaultAmmoData();
	FAmmoData CurrentAmmoData = RestockedWeapon->GetCurrentAmmoData();

	if (CurrentAmmoData.bInfiniteAmmo) {
		return 0;
	}

	int32 MaxBullets = DefaultAmmoData.BulletsInClip + DefaultAmmoData.BulletsInClip * DefaultAmmoData.Clips;
	int32 CurrentBullets = CurrentAmmoData.BulletsInClip + CurrentAmmoData.SpareBullets;
	int32 AmmoTriedToAdd = 0;

	switch (AmmoRestoreType) {
	case EAmmoRestoreType::ClipsOnly:
		AmmoTriedToAdd = DefaultAmmoData.BulletsInClip * ClipsAdded;
		break;

	case EAmmoRestoreType::BulletsOnly:
		AmmoTriedToAdd = BulletsAdded;
		break;

	case EAmmoRestoreType::ClipsAndBullets:
		AmmoTriedToAdd = DefaultAmmoData.BulletsInClip * ClipsAdded + BulletsAdded;
		break;

	default:
		AmmoTriedToAdd = 0;
		break;
	}

	int32 AmmoCanBeAdded = FMath::Min(MaxBullets - CurrentBullets, AmmoTriedToAdd);

	UE_LOG(
		LogShooterWeaponComponent, Display, TEXT("Ammo Restock Log: Weapon: %s, Max: %d, Current: %d, TriedToAdd: %d, CanBeAdded: %d"),
		*RestockedWeapon->GetName(), MaxBullets, CurrentBullets, AmmoTriedToAdd, AmmoCanBeAdded
	);

	return AmmoCanBeAdded;
}

void UShooterWeaponComponent::OnEquipFinished(
	USkeletalMeshComponent *MeshComp
)
{
	if (GetOwnerSkeletalMesh() != MeshComp) {
		return;
	}

	UE_LOG(LogShooterWeaponComponent, Display, TEXT("Equip Finished!"));
	bEquipAnimInProgress = false;

	if (CurrentWeapon && CurrentWeapon->IsClipEmpty() && CanReload()) {
		Reload();
	}
}

void UShooterWeaponComponent::OnReloadFinished(
	USkeletalMeshComponent *MeshComp
)
{
	if (GetOwnerSkeletalMesh() != MeshComp) {
		return;
	}

	UE_LOG(LogShooterWeaponComponent, Display, TEXT("Reload Finished!"));
	bReloadAnimInProgress = false;

	if (!CanReload()) {
		return;
	}

	CurrentWeapon->Reload();
}

void UShooterWeaponComponent::OnEmptyClip(
)
{
	Reload();
}
