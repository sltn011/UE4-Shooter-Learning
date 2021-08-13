// Shooter, All Rights Reserved


#include "Player/ShooterPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/ShooterWeaponComponent.h"
#include "GameFramework/SpringArmComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterPlayerCharacter, All, All);

AShooterPlayerCharacter::AShooterPlayerCharacter(
    FObjectInitializer const &ObjInitializer
)
    : Super{ ObjInitializer }
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->AddRelativeLocation(FVector{ 0.0f, 0.0f, 65.0f });
	SpringArmComponent->SocketOffset = FVector{ 0.0f, 100.0f, 0.0f };

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
}

void AShooterPlayerCharacter::SetupPlayerInputComponent(
	UInputComponent *PlayerInputComponent
)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!PlayerInputComponent || !WeaponComponent || !GetCapsuleComponent()) {
		UE_LOG(
			LogShooterPlayerCharacter,
			Error,
			TEXT("Error binding input actions! PlayerInputComponent: %d, WeaponComponent: %d, CapsuleComponent: %d"),
			PlayerInputComponent, WeaponComponent, GetCapsuleComponent()
		);
		checkNoEntry();
	}

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("TurnAround", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AShooterBaseCharacter::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AShooterBaseCharacter::StopRunning);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AShooterBaseCharacter::StartShooting);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AShooterBaseCharacter::StopShooting);

	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::EquipNextWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::ReloadWeapon);
}

void AShooterPlayerCharacter::BeginPlay(
)
{
	Super::BeginPlay();

	check(SpringArmComponent);
	check(CameraComponent);
}

void AShooterPlayerCharacter::OnDeath(
)
{
	Super::OnDeath();

	if (Controller) {
		Controller->ChangeState(NAME_Spectating);
	}
}

void AShooterPlayerCharacter::MoveForward(
	float Scale
)
{
	bIsMovingForward = Scale > 0.0f;
	AddMovementInput(GetActorForwardVector(), Scale);
}

void AShooterPlayerCharacter::MoveRight(
	float Scale
)
{
	AddMovementInput(GetActorRightVector(), Scale);
}
