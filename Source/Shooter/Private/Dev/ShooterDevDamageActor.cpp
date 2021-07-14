// Shooter, All Rights Reserved


#include "Dev/ShooterDevDamageActor.h"

#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AShooterDevDamageActor::AShooterDevDamageActor(
)
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
}

void AShooterDevDamageActor::BeginPlay(
)
{
	Super::BeginPlay();
	
}

void AShooterDevDamageActor::Tick(
	float DeltaTime
)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, bDoFullDamage);
}

