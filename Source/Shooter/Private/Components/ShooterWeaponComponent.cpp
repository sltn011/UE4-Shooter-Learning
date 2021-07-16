// Shooter, All Rights Reserved


#include "Components/ShooterWeaponComponent.h"

#include "Player/ShooterBaseCharacter.h"
#include "Weapon/ShooterBaseWeapon.h"

UShooterWeaponComponent::UShooterWeaponComponent(
)
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UShooterWeaponComponent::StartShooting(
)
{
	if (!CurrentWeapon) {
		return;
	}
	CurrentWeapon->StartShooting();
}

void UShooterWeaponComponent::StopShooting(
)
{
	if (!CurrentWeapon) {
		return;
	}
	CurrentWeapon->StopShooting();
}

void UShooterWeaponComponent::BeginPlay(
)
{
	Super::BeginPlay();
	
	SpawnWeapon();
	AShooterBaseCharacter *OwnerCharacter = Cast<AShooterBaseCharacter>(GetOwner());
	if (!OwnerCharacter) {
		return;
	}

	OwnerCharacter->OnOwnerDeath.BindUObject(this, &UShooterWeaponComponent::OnOwnerDeath);
	OwnerCharacter->OnOwnerDespawn.BindUObject(this, &UShooterWeaponComponent::OnOwnerDespawn);
}

void UShooterWeaponComponent::SpawnWeapon(
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

	USkeletalMeshComponent *SkeletalMesh = Character->GetMesh();
	if (!SkeletalMesh) {
		return;
	}

	CurrentWeapon = World->SpawnActor<AShooterBaseWeapon>(WeaponClass);
	if (!CurrentWeapon) {
		return;
	}

	FAttachmentTransformRules AttachmentRules{ EAttachmentRule::SnapToTarget, false };
	CurrentWeapon->AttachToComponent(SkeletalMesh, AttachmentRules, WeaponAttachSocketName);
	CurrentWeapon->SetOwner(Character);
}

void UShooterWeaponComponent::OnOwnerDeath(
)
{
	StopShooting();
}

void UShooterWeaponComponent::OnOwnerDespawn(
)
{
	if (!CurrentWeapon) {
		return;
	}
	CurrentWeapon->Destroy();
}