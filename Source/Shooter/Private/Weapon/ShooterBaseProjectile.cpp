// Shooter, All Rights Reserved


#include "Weapon/ShooterBaseProjectile.h"

#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Weapon/Components/ShooterWeaponFXComponent.h"

AShooterBaseProjectile::AShooterBaseProjectile(
)
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComponent"));
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComponent->bReturnMaterialOnMove = true;
	SetRootComponent(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = 2500.0f;
	MovementComponent->ProjectileGravityScale = 0.0f;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(GetRootComponent());

	WeaponFXComponent = CreateDefaultSubobject<UShooterWeaponFXComponent>(TEXT("WeaponFXComponent"));
}

void AShooterBaseProjectile::BeginPlay(
)
{
	Super::BeginPlay();

	check(CollisionComponent);
	check(MovementComponent);
	check(AudioComponent);
	check(WeaponFXComponent);

	check(TimeToLive > 0.0f);
	check(DamageAmount >= 0.0f);
	check(AreaRadius > 0.0f);

	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponent->OnComponentHit.AddDynamic(this, &AShooterBaseProjectile::OnHit);

	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;

	AudioComponent->Play();

	SetLifeSpan(TimeToLive);
}

void AShooterBaseProjectile::OnHit(
	UPrimitiveComponent *HitComponent,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComp,
	FVector NormalImpulse,
	FHitResult const &Hit
)
{
	UWorld *World = GetWorld();
	if (!World || !MovementComponent || !WeaponFXComponent) {
		return;
	}

	MovementComponent->StopMovementImmediately();

	if (AudioComponent) {
		AudioComponent->Stop();
	}

	FVector ImpactLocation = GetActorLocation();
	
	AtImpactLocation(ImpactLocation, Hit);
}

void AShooterBaseProjectile::AtImpactLocation(
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

	DrawDebugSphere(World, ImpactLocation, AreaRadius, 24, FColor::Cyan, false, 3.0f);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, ImpactLocation);

	Destroy();
}

AController *AShooterBaseProjectile::GetOwnerController(
) const
{
	APawn const *Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}

