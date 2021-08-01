// Shooter, All Rights Reserved


#include "AI/Tasks/ShooterNextLocationTask.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterNextLocationTask, All, All);

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

    FVector SearchAreaCenter = Pawn->GetActorLocation();
    if (!bSelfCentered) {
        AActor *CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
        if (!CenterActor) {
            return EBTNodeResult::Failed;
        }
        SearchAreaCenter = CenterActor->GetActorLocation();
    }

    FNavLocation NextLocation;
    bool const bPointFound = NavSystem->GetRandomReachablePointInRadius(
        SearchAreaCenter,
        LocationSearchRadius,
        NextLocation
    );
    if (!bPointFound) {
        UE_LOG(LogShooterNextLocationTask, Display, TEXT("Next location not found for %s"), *Pawn->GetName());
        return EBTNodeResult::Failed;
    }

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NextLocation.Location);

    return EBTNodeResult::Succeeded;
}
