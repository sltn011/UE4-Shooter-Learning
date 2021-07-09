// Shooter, All Rights Reserved


#include "Player/ShooterBaseCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AShooterBaseCharacter::AShooterBaseCharacter()
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

	if (PlayerInputComponent) {
		PlayerInputComponent->BindAxis("MoveForward", this, &AShooterBaseCharacter::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &AShooterBaseCharacter::MoveRight);

		PlayerInputComponent->BindAxis("TurnAround", this, &APawn::AddControllerYawInput);
		PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	}
}

void AShooterBaseCharacter::MoveForward(float Scale)
{
	AddMovementInput(GetActorForwardVector(), Scale);
}

void AShooterBaseCharacter::MoveRight(float Scale)
{
	AddMovementInput(GetActorRightVector(), Scale);
}
