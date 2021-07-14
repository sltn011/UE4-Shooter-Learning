// Shooter, All Rights Reserved


#include "Components/ShooterWeaponComponent.h"

#include "Weapon/ShooterBaseWeapon.h"
#include "Player/ShooterBaseCharacter.h"

UShooterWeaponComponent::UShooterWeaponComponent(
)
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UShooterWeaponComponent::Shoot(
)
{
	if (!CurrentWeapon) {
		return;
	}
	CurrentWeapon->Shoot();
}

void UShooterWeaponComponent::BeginPlay(
)
{
	Super::BeginPlay();
	
	SpawnWeapon();
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
}