// Shooter, All Rights Reserved


#include "Weapon/ShooterBaseWeapon.h"

#include "Components/SkeletalMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

AShooterBaseWeapon::AShooterBaseWeapon(
)
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
}

void AShooterBaseWeapon::BeginPlay(
)
{
	Super::BeginPlay();
	
}

void AShooterBaseWeapon::Shoot()
{
	UE_LOG(LogBaseWeapon, Display, TEXT("Shoot!"));
}

