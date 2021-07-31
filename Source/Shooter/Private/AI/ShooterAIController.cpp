// Shooter, All Rights Reserved


#include "AI/ShooterAIController.h"

#include "BehaviorTree/BehaviorTree.h"

void AShooterAIController::BeginPlay(
)
{
    Super::BeginPlay();

    if (BehaviorTree) {
        RunBehaviorTree(BehaviorTree);
    }
}