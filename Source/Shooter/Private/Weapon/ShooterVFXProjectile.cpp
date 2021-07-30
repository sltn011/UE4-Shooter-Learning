// Shooter, All Rights Reserved


#include "Weapon/ShooterVFXProjectile.h"

#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Weapon/Components/ShooterWeaponFXComponent.h"

AShooterVFXProjectile::AShooterVFXProjectile(
)
{
    LightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComponent"));
    LightComponent->SetLightColor(FColor{ 0, 170, 255 });
    LightComponent->SetupAttachment(GetRootComponent());
}

void AShooterVFXProjectile::BeginPlay(
)
{
    Super::BeginPlay();

    check(ProjectileFX);
    check(AfterImpactLifeTime >= 0.0f);

    InitProjectileFX();
}

void AShooterVFXProjectile::AtImpactLocation(
    FVector const &ImpactLocation,
    FHitResult const &Hit
)
{
    UWorld *World = GetWorld();
    if (!World) {
        return;
    }

    UGameplayStatics::ApplyRadialDamage(World, DamageAmount, ImpactLocation, AreaRadius, UDamageType::StaticClass(), {}, this, GetOwnerController(), DoFullDamage);

    WeaponFXComponent->PlayImpactFX(Hit);

    AfterImpactLifeTime > 0.0f ? OnImpactDeactivate() : Destroy();
}

void AShooterVFXProjectile::InitProjectileFX(
)
{
    if (ProjectileFXComponent || !ProjectileFX || !MovementComponent) {
        return;
    }

    ProjectileFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
        ProjectileFX,
        GetRootComponent(),
        NAME_None,
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        EAttachLocation::KeepRelativeOffset,
        false
    );
}

void AShooterVFXProjectile::OnImpactDeactivate(
)
{
    ProjectileFXComponent->Deactivate();
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    LightComponent->SetLightColor(FLinearColor::Black);
    LightComponent->bAffectsWorld = false;
    SetLifeSpan(AfterImpactLifeTime);
}
