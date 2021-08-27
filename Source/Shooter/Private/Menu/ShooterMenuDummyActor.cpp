// Shooter, All Rights Reserved


#include "Menu/ShooterMenuDummyActor.h"

#include "Components/SkeletalMeshComponent.h"
#include "Weapon/ShooterBaseWeapon.h"

AShooterMenuDummyActor::AShooterMenuDummyActor(
)
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SetRootComponent(SkeletalMeshComponent);
}

void AShooterMenuDummyActor::BeginPlay(
)
{
	Super::BeginPlay();
	
	check(SkeletalMeshComponent);

	UWorld *World = GetWorld();
	if (!World) {
		return;
	}

	AShooterBaseWeapon *Weapon = Cast<AShooterBaseWeapon>(World->SpawnActor(WeaponClass));
	if (!Weapon) {
		return;
	}

	Weapon->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
}

