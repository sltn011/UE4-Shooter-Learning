// Shooter, All Rights Reserved


#include "AI/ShooterAICharacter.h"

#include "AI/ShooterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/ShooterAIWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AShooterAICharacter::AShooterAICharacter(
	FObjectInitializer const &ObjInit
)
	: Super{ObjInit.SetDefaultSubobjectClass<UShooterAIWeaponComponent>(TEXT("WeaponComponent"))}
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = AShooterAIController::StaticClass();

	bUseControllerRotationYaw = false;

	UCharacterMovementComponent *MovementComponent = GetCharacterMovement();
	if (MovementComponent) {
		MovementComponent->RotationRate = FRotator{ 0.0f, 200.0f, 0.0f };
		MovementComponent->bUseControllerDesiredRotation = true;
	}
}

void AShooterAICharacter::OnDeath(
)
{
	AAIController *OwnerController = Cast<AAIController>(GetController());
	if (OwnerController) {
		UBrainComponent *Brain = OwnerController->GetBrainComponent();
		if (Brain) {
			Brain->Cleanup();
		}
	}
	Super::OnDeath();
}
