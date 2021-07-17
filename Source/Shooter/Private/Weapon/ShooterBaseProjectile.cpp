// Shooter, All Rights Reserved


#include "Weapon/ShooterBaseProjectile.h"

#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AShooterBaseProjectile::AShooterBaseProjectile(
)
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComponent"));
	SphereCollisionComponent->InitSphereRadius(5.0f);
	SphereCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SetRootComponent(SphereCollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = 2500.0f;
	MovementComponent->ProjectileGravityScale = 0.0f;
}

void AShooterBaseProjectile::BeginPlay(
)
{
	Super::BeginPlay();

	SphereCollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	SphereCollisionComponent->OnComponentHit.AddDynamic(this, &AShooterBaseProjectile::OnHit);

	if (MovementComponent) {
		MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	}

	SetLifeSpan(TimeToLive);
}

void AShooterBaseProjectile::OnHit(
	UPrimitiveComponent *HitComponent,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComp,
	FVector NormalImpulse,
	const FHitResult &Hit
)
{
	UWorld *World = GetWorld();
	if (!World || !MovementComponent) {
		return;
	}

	MovementComponent->StopMovementImmediately();

	FVector ProjectileLocation = GetActorLocation();
	
	UGameplayStatics::ApplyRadialDamage(World, DamageAmount, ProjectileLocation, AreaRadius, UDamageType::StaticClass(), {}, this, GetOwnerController(), DoFullDamage);

	DrawDebugSphere(World, ProjectileLocation, AreaRadius, 24, FColor::Cyan, false, 3.0f);

	Destroy();
}

AController *AShooterBaseProjectile::GetOwnerController(
) const
{
	APawn const *Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}

