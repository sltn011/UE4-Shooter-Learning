// Shooter, All Rights Reserved


#include "Player/ShooterBaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ShooterCharMovementComponent.h"
#include "Components/ShooterHealthComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterBaseCharacter, All, All);

AShooterBaseCharacter::AShooterBaseCharacter(
	FObjectInitializer const &ObjInitializer
)
	: Super{ObjInitializer.SetDefaultSubobjectClass<UShooterCharMovementComponent>(ACharacter::CharacterMovementComponentName)}
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->AddRelativeLocation(FVector{ 0.0f, 0.0f, 65.0f });
	SpringArmComponent->SocketOffset = FVector{ 0.0f, 75.0f, 0.0f };

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

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

void AShooterBaseCharacter::BeginPlay(
)
{
	Super::BeginPlay();

	check(SpringArmComponent);
	check(CameraComponent);
	check(HealthComponent);
	check(HealthTextComponent);
	check(WeaponComponent);

	check(LifeSpanAfterDeath >= 0.0f);

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnHealthChanged.AddUObject(this, &AShooterBaseCharacter::OnHealthChanged);
	HealthComponent->OnDeath.AddUObject(this, &AShooterBaseCharacter::OnDeath);

	LandedDelegate.AddDynamic(this, &AShooterBaseCharacter::OnGroundLanding);
}

void AShooterBaseCharacter::EndPlay(
	EEndPlayReason::Type const EndPlayReason
)
{
	Super::EndPlay(EndPlayReason);
}

void AShooterBaseCharacter::Tick(
	float DeltaTime
)
{
	Super::Tick(DeltaTime); 
}

void AShooterBaseCharacter::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent
)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!PlayerInputComponent || !WeaponComponent || !GetCapsuleComponent()) {
		UE_LOG(
			LogShooterBaseCharacter,
			Error,
			TEXT("Error binding input actions! PlayerInputComponent: %d, WeaponComponent: %d, CapsuleComponent: %d"),
			PlayerInputComponent, WeaponComponent, GetCapsuleComponent()
		);
		checkNoEntry();
	}

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterBaseCharacter::MoveRight);

	PlayerInputComponent->BindAxis("TurnAround", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AShooterBaseCharacter::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AShooterBaseCharacter::StopRunning);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::StartShooting);
	PlayerInputComponent->BindAction("Shoot", IE_Released, WeaponComponent, &UShooterWeaponComponent::StopShooting);

	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::EquipNextWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::ReloadWeapon);
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

void AShooterBaseCharacter::MoveForward(
	float Scale
)
{
	bIsMovingForward = Scale > 0.0f;
	AddMovementInput(GetActorForwardVector(), Scale);
}

void AShooterBaseCharacter::MoveRight(
	float Scale
)
{
	AddMovementInput(GetActorRightVector(), Scale);
}

void AShooterBaseCharacter::StartRunning(
)
{
	bIsRunning = true;
}

void AShooterBaseCharacter::StopRunning(
)
{
	bIsRunning = false;
}

void AShooterBaseCharacter::OnHealthChanged(
	float NewHealth
)
{
	if (HealthTextComponent) {
		HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), NewHealth)));
	}
}

void AShooterBaseCharacter::OnDeath(
)
{
	//if (DeathAnimMontage) - Already exists in PlayAnimMontage
	PlayAnimMontage(DeathAnimMontage);
	
	if (Controller) {
		Controller->ChangeState(NAME_Spectating);
	}

	UCapsuleComponent *Capsule = GetCapsuleComponent();
	if (Capsule) {
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		Capsule->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}

	if (WeaponComponent) {
		WeaponComponent->StopShooting();
	}

	SetLifeSpan(LifeSpanAfterDeath);
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
