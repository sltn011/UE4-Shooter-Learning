// Shooter, All Rights Reserved


#include "AI/Contexts/ShooterEnemyEnvQueryContext.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"


void UShooterEnemyEnvQueryContext::ProvideContext(
	FEnvQueryInstance &QueryInstance,
	FEnvQueryContextData &ContextData
) const
{
	AActor *QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());

	UBlackboardComponent *Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);
	if (!Blackboard) {
		return;
	}

	AActor *EnemyActor = Cast<AActor>(Blackboard->GetValueAsObject(EnemyActorKeyName));

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, EnemyActor);
}