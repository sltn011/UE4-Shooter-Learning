// Shooter, All Rights Reserved


#include "Player/ShooterBaseCharacter.h"

#include "Components/ShooterCharMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterBaseCharacter, All, All)

// Sets default values
AShooterBaseCharacter::AShooterBaseCharacter(FObjectInitializer const &ObjInitializer)
	: Super{ObjInitializer.SetDefaultSubobjectClass<UShooterCharMovementComponent>(ACharacter::CharacterMovementComponentName)}
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void AShooterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShooterBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!PlayerInputComponent) {
		return;
	}

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterBaseCharacter::MoveRight);

	PlayerInputComponent->BindAxis("TurnAround", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AShooterBaseCharacter::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AShooterBaseCharacter::StopRunning);
}

bool AShooterBaseCharacter::IsRunning() const
{
	return bIsRunning && bIsMovingForward && !GetVelocity().IsNearlyZero();
}

float AShooterBaseCharacter::MoveDirectionRadians() const
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

void AShooterBaseCharacter::MoveForward(float Scale)
{
	bIsMovingForward = Scale > 0.0f;
	AddMovementInput(GetActorForwardVector(), Scale);
}

void AShooterBaseCharacter::MoveRight(float Scale)
{
	AddMovementInput(GetActorRightVector(), Scale);
}

void AShooterBaseCharacter::StartRunning()
{
	bIsRunning = true;
}

void AShooterBaseCharacter::StopRunning()
{
	bIsRunning = false;
}
