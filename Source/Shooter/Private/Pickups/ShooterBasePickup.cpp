// Shooter, All Rights Reserved


#include "Pickups/ShooterBasePickup.h"

#include "Components/SphereComponent.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterBasePickup, All, All)

AShooterBasePickup::AShooterBasePickup(
)
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(CollisionComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(GetRootComponent());
}

void AShooterBasePickup::Tick(
	float DeltaTime
)
{
	Super::Tick(DeltaTime);

	if (bIsRotating) {
		AddActorLocalRotation(FRotator{ 0.0f, PickupYawRotation, 0.0f });
	}
}

void AShooterBasePickup::BeginPlay(
)
{
	Super::BeginPlay();

	check(CollisionComponent);
	check(MeshComponent);

	check(RespawnTime >= 0.0f);
	check(DelayBeforePickingAfterRespawn >= 0.0f);

	if (bIsRotating) {
		PickupYawRotation = bIsRandomRotationSpeed ? GenerateRotationYawSpeed() : RotationYawSpeed;
	}
	else {
		PickupYawRotation = 0.0f;
	}
}

void AShooterBasePickup::NotifyActorBeginOverlap(
	AActor *PickerActor
)
{
	Super::NotifyActorBeginOverlap(PickerActor);

	APawn *PickerPawn = Cast<APawn>(PickerActor);
	if (!PickerPawn) {
		return;
	}

	UE_LOG(LogShooterBasePickup, Display, TEXT("Pickup %s taken by %s"), *GetName(), PickerPawn ? *PickerPawn->GetName() : TEXT("NULL-Pawn"));
	PickupEffect(PickerPawn);

	OnBeingPicked();
}

void AShooterBasePickup::PickupEffect(
	APawn *PickerPawn
)
{
}

void AShooterBasePickup::OnBeingPicked(
)
{
	bIsRespawnable ? DisappearFromWorld() : Destroy();
}

void AShooterBasePickup::DisappearFromWorld(
)
{
	USceneComponent *PickupRoot = GetRootComponent();
	if (!CollisionComponent || !PickupRoot) {
		return;
	}

	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	PickupRoot->SetVisibility(false, true);

	FTimerHandle ReappearTimerHandle;
	GetWorldTimerManager().SetTimer(ReappearTimerHandle, this, &AShooterBasePickup::ReappearInWorld, RespawnTime, false);
}

void AShooterBasePickup::ReappearInWorld(
)
{
	USceneComponent *PickupRoot = GetRootComponent();
	if (!CollisionComponent || !PickupRoot) {
		return;
	}

	if (bIsRotating) {
		PickupYawRotation = bIsRandomRotationSpeed ? GenerateRotationYawSpeed() : RotationYawSpeed;
	}
	else {
		PickupYawRotation = 0.0f;
	}

	PickupRoot->SetVisibility(true, true);

	UE_LOG(LogShooterBasePickup, Display, TEXT("Pickup %s reappeared!"), *GetName());

	// If has delay before being pickable after respawn
	if (DelayBeforePickingAfterRespawn > 0.0f) {
		FTimerHandle Delay;
		FTimerDelegate EnableCollisionAfterDelay;
		EnableCollisionAfterDelay.BindUObject(CollisionComponent, &UPrimitiveComponent::SetCollisionResponseToAllChannels, ECollisionResponse::ECR_Overlap);
		GetWorldTimerManager().SetTimer(Delay, EnableCollisionAfterDelay, DelayBeforePickingAfterRespawn, false);
	}
	else {
		CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	}
}

float AShooterBasePickup::GenerateRotationYawSpeed(
)
{
	bool Direction = FMath::RandBool();
	return FMath::RandRange(1.0f, 3.0f) * Direction ? 1.0f : -1.0f;
}
