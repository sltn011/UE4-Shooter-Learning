// Shooter, All Rights Reserved


#include "AI/Services/ShooterFindEnemyService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ShooterAIPerceptionComponent.h"
#include "ShooterUtils.h"

UShooterFindEnemyService::UShooterFindEnemyService(
)
{
    NodeName = "Find Enemy";
}

void UShooterFindEnemyService::TickNode(
    UBehaviorTreeComponent &OwnerComp,
    uint8 *NodeMemory,
    float DeltaSeconds
)
{
    UBlackboardComponent *Blackboard = OwnerComp.GetBlackboardComponent();
    if (Blackboard) {
        AAIController *Controller = OwnerComp.GetAIOwner();
        if (Controller) {
            UShooterAIPerceptionComponent *PerceptionComponent = ShooterUtils::GetPlayerComponentByClass<UShooterAIPerceptionComponent>(Controller);
            if (PerceptionComponent) {
                Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestVisibleEnemyActor());
            }
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
