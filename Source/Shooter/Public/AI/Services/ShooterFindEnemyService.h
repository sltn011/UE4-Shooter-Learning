// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ShooterFindEnemyService.generated.h"


UCLASS()
class SHOOTER_API UShooterFindEnemyService : public UBTService
{

	GENERATED_BODY()
	
public:

	UShooterFindEnemyService(
	);

protected:

	virtual void TickNode(
		UBehaviorTreeComponent &OwnerComp,
		uint8 *NodeMemory,
		float DeltaSeconds
	) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	FBlackboardKeySelector EnemyActorKey;

};
