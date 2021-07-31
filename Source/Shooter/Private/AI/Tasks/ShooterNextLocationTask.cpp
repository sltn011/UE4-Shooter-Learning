// Shooter, All Rights Reserved


#include "AI/Tasks/ShooterNextLocationTask.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UShooterNextLocationTask::UShooterNextLocationTask(
)
{
    NodeName = "Next Location";
}

EBTNodeResult::Type UShooterNextLocationTask::ExecuteTask(
    UBehaviorTreeComponent &OwnerComp,
    uint8 *NodeMemory
)
{
    AAIController const *Controller = OwnerComp.GetAIOwner();
    UBlackboardComponent *Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) {
        return EBTNodeResult::Failed;
    }

    APawn *Pawn = Controller->GetPawn();
    if (!Pawn) {
        return EBTNodeResult::Failed;
    }

    UNavigationSystemV1 *NavSystem = UNavigationSystemV1::GetCurrent(Pawn);
    if (!NavSystem) {
        return EBTNodeResult::Failed;
    }

    FNavLocation NextLocation;
    bool const bPointFound = NavSystem->GetRandomReachablePointInRadius(
        Pawn->GetActorLocation(),
        LocationSearchRadius,
        NextLocation
    );
    if (!bPointFound) {
        return EBTNodeResult::Failed;
    }

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NextLocation.Location);

    return EBTNodeResult::Succeeded;
}
