// Shooter, All Rights Reserved


#include "AI/ShooterAICharacter.h"

#include "AI/ShooterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"

AShooterAICharacter::AShooterAICharacter(
	FObjectInitializer const &ObjInit
)
	: Super{ObjInit}
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AShooterAIController::StaticClass();

	bUseControllerRotationYaw = false;

	UCharacterMovementComponent *MovementComponent = GetCharacterMovement();
	if (MovementComponent) {
		MovementComponent->RotationRate = FRotator{ 0.0f, 200.0f, 0.0f };
		MovementComponent->bUseControllerDesiredRotation = true;
	}
}
