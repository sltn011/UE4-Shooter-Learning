// Shooter, All Rights Reserved


#include "AI/ShooterAIController.h"

#include "AI/ShooterAICharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ShooterAIPerceptionComponent.h"
#include "Components/ShooterRespawnComponent.h"

AShooterAIController::AShooterAIController(
)
{
	AIPerceptionComponent = CreateDefaultSubobject<UShooterAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	RespawnComponent = CreateDefaultSubobject<UShooterRespawnComponent>(TEXT("RespawnComponent"));

	bWantsPlayerState = true;
}

void AShooterAIController::OnPossess(
	APawn *PossessedPawn
)
{
	Super::OnPossess(PossessedPawn);

	AShooterAICharacter *AICharacter = Cast<AShooterAICharacter>(PossessedPawn);
	if (AICharacter && AICharacter->BehaviorTreeAsset) {
		RunBehaviorTree(AICharacter->BehaviorTreeAsset);
	}
}

void AShooterAIController::Tick(
	float DeltaTime
)
{
	Super::Tick(DeltaTime);

	AActor *ClosestActor = GetFocusOnActor();
	SetFocus(ClosestActor);
}

AActor *AShooterAIController::GetFocusOnActor(
) const
{
	UBlackboardComponent const *BlackboardComponent = GetBlackboardComponent();
	if (!BlackboardComponent) {
		return nullptr;
	}
	return Cast<AActor>(BlackboardComponent->GetValueAsObject(FocusOnActorKeyName));
}
