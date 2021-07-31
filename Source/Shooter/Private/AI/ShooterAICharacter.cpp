// Shooter, All Rights Reserved


#include "AI/ShooterAICharacter.h"

#include "AI/ShooterAIController.h"

AShooterAICharacter::AShooterAICharacter(
	FObjectInitializer const &ObjInit
)
	: Super{ObjInit}
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AShooterAIController::StaticClass();
}