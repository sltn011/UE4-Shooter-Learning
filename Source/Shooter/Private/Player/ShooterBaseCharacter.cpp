// Shooter, All Rights Reserved


#include "Player/ShooterBaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/ShooterCharMovementComponent.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "Player/ShooterPlayerState.h"
#include "ShooterGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterBaseCharacter, All, All);

AShooterBaseCharacter::AShooterBaseCharacter(
	FObjectInitializer const &ObjInitializer
)
	: Super{ ObjInitializer.SetDefaultSubobjectClass<UShooterCharMovementComponent>(ACharacter::CharacterMovementComponentName) }
{
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UShooterHealthComponent>(TEXT("HealthComponent"));

	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthTextComponent"));
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->Text = FText::FromString(FString{ TEXT("0") });
	HealthTextComponent->TextRenderColor = FColor::Red;
	HealthTextComponent->AddLocalOffset({ 0.0f, 0.0f, 100.0f });
	HealthTextComponent->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	HealthTextComponent->bOwnerNoSee = true;

	WeaponComponent = CreateDefaultSubobject<UShooterWeaponComponent>(TEXT("WeaponComponent"));
}

void AShooterBaseCharacter::StartRunning(
)
{
	if (WeaponComponent->IsShooting()) {
		WeaponComponent->StopShooting(); // Can't shoot while running
	}
	bIsRunning = true;
}

void AShooterBaseCharacter::StopRunning(
)
{
	bIsRunning = false;
}

void AShooterBaseCharacter::StartShooting(
)
{
	bIsRunning = false; // Can't shoot while running
	WeaponComponent->StartShooting();
}

void AShooterBaseCharacter::StopShooting(
)
{
	WeaponComponent->StopShooting();
}

bool AShooterBaseCharacter::IsDead(
) const
{
	if (!HealthComponent || HealthComponent->IsDead()) {
		return true;
	}
	return false;
}

bool AShooterBaseCharacter::IsRunning(
) const
{
	return bIsRunning && bIsMovingForward && !GetVelocity().IsNearlyZero();
}

float AShooterBaseCharacter::MoveDirectionRadians(
) const
{
	if (GetVelocity().IsZero()) {
		return 0.0f;
	}

	FVector const Forward = GetActorForwardVector();
	FVector const VelocityNormal = GetVelocity().GetSafeNormal();

	FVector const Orthogonal = FVector::CrossProduct(Forward, VelocityNormal);

	float const DirectionSign = FMath::Sign(Orthogonal.Z);
	float const RadiansBetween = FMath::Acos(FVector::DotProduct(Forward, VelocityNormal));

	return Orthogonal.IsZero() ? RadiansBetween : DirectionSign * RadiansBetween;
}

void AShooterBaseCharacter::SetPlayerColor(
	FLinearColor const &Color
)
{
	USkeletalMeshComponent *PlayerMesh = GetMesh();
	if (!PlayerMesh) {
		return;
	}

	UMaterialInstanceDynamic *MaterialInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	if (!MaterialInstance) {
		return;
	}

	MaterialInstance->SetVectorParameterValue(MaterialColorName, Color);
}

void AShooterBaseCharacter::BeginPlay(
)
{
	Super::BeginPlay();

	check(HealthComponent);
	check(HealthTextComponent);
	check(WeaponComponent);

	check(LifeSpanAfterDeath >= 0.0f);

	OnHealthChanged(HealthComponent->GetHealth(), HealthComponent->GetHealth());
	HealthComponent->OnHealthChanged.AddUObject(this, &AShooterBaseCharacter::OnHealthChanged);
	HealthComponent->OnDeath.AddUObject(this, &AShooterBaseCharacter::OnDeath);

	LandedDelegate.AddDynamic(this, &AShooterBaseCharacter::OnGroundLanding);
}

void AShooterBaseCharacter::OnDeath(
)
{
	UCapsuleComponent *Capsule = GetCapsuleComponent();
	if (Capsule) {
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		Capsule->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}

	if (WeaponComponent) {
		WeaponComponent->StopShooting();
	}

	USkeletalMeshComponent *PlayerMesh = GetMesh();
	if (PlayerMesh) {
		PlayerMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PlayerMesh->SetSimulatePhysics(true);
	}

	if (HealthTextComponent) {
		HealthTextComponent->SetText(FText::GetEmpty());
	}

	SetLifeSpan(LifeSpanAfterDeath);
}

void AShooterBaseCharacter::OnHealthChanged(
	float NewHealth,
	float HealthDelta
)
{
	if (HealthTextComponent) {
		HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), NewHealth)));
	}
}

void AShooterBaseCharacter::OnGroundLanding(
	FHitResult const &HitResult
)
{
	float const FallVelocity = -1 * GetVelocity().Z; // Negate to make value notnegative

	float const MinDamagingVelocity = MinMaxDamagingFallVelocities.X;
	float const MaxDamagingVelocity = MinMaxDamagingFallVelocities.Y;

	if (MinDamagingVelocity <= FallVelocity) {

		float const FallDamage = FMath::GetMappedRangeValueClamped(MinMaxDamagingFallVelocities, MinMaxFallDamage, FallVelocity);

		TakeDamage(FallDamage, {}, nullptr, nullptr);
	}
}

void AShooterBaseCharacter::FellOutOfWorld(
	UDamageType const &DmgType
)
{
	UWorld *World = GetWorld();
	if (World) {

		AController *OwnerController = GetController();

		if (OwnerController) {

			AShooterPlayerState *OwnerPlayerState = OwnerController->GetPlayerState<AShooterPlayerState>();
			if (OwnerPlayerState) {

				OwnerPlayerState->AddDeath();

				AShooterGameModeBase *GameMode = World->GetAuthGameMode<AShooterGameModeBase>();
				if (GameMode && GameMode->IsRespawningEnabled()) {
					GameMode->RespawnAfterDelay(OwnerController);
				}

			}

		}

	}

	OnDeath();

	Super::FellOutOfWorld(DmgType);
}
