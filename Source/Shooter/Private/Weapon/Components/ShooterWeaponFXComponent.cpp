// Shooter, All Rights Reserved


#include "Weapon/Components/ShooterWeaponFXComponent.h"

#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

UShooterWeaponFXComponent::UShooterWeaponFXComponent(
)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UShooterWeaponFXComponent::PlayImpactFX(
	FHitResult const &Hit
)
{
	UWorld *World = GetWorld();
	if (!World) {
		return;
	}

	FImpactData ImpactData = DefaultImpactData;

	if (Hit.PhysMaterial.IsValid()) {
		UPhysicalMaterial *PhysMaterial = Hit.PhysMaterial.Get();
		if (ImpactDataMap.Contains(PhysMaterial)) {
			ImpactData = ImpactDataMap[PhysMaterial];
		}
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, ImpactData.Effect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
	UDecalComponent *Decal = UGameplayStatics::SpawnDecalAtLocation(
		World, 
		ImpactData.ImpactDecal.Material, 
		ImpactData.ImpactDecal.Size,
		Hit.ImpactPoint,
		Hit.ImpactNormal.Rotation()
	);

	if (!Decal) {
		return;
	}

	Decal->SetFadeOut(ImpactData.ImpactDecal.LifeTime, ImpactData.ImpactDecal.FadeOutTime);
}

void UShooterWeaponFXComponent::BeginPlay(
)
{
	Super::BeginPlay();
}

