// Shooter, All Rights Reserved


#include "Weapon/ShooterMeshProjectile.h"

#include "Components/StaticMeshComponent.h"

AShooterMeshProjectile::AShooterMeshProjectile(
)
{
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(GetRootComponent());
}

void AShooterMeshProjectile::BeginPlay(
)
{
    Super::BeginPlay();

    check(ProjectileMesh);
}
