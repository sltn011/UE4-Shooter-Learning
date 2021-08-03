// Shooter, All Rights Reserved


#include "AI/Services/ShooterShootService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/ShooterBaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterShooterService, All, All);

UShooterShootService::UShooterShootService(
)
{
	NodeName = "Shoot";
}

void UShooterShootService::TickNode(
	UBehaviorTreeComponent &OwnerComp,
	uint8 *NodeMemory,
	float DeltaSeconds
)
{
	UBlackboardComponent *Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) {
		return;
	}

	AAIController *Controller = OwnerComp.GetAIOwner();
	if (!Controller) {
		return;
	}

	AShooterBaseCharacter *ShooterCharacter = Cast<AShooterBaseCharacter>(Controller->GetPawn());
	if (!ShooterCharacter) {
		return;
	}

	AActor *TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
	//UE_LOG(LogShooterShooterService, Display, TEXT("Target actor: %s"), TargetActor ? *TargetActor->GetName() : TEXT("None"));
	
	TargetActor ? ShooterCharacter->StartShooting() : ShooterCharacter->StopShooting();

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}