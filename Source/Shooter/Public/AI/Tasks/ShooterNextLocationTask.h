// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ShooterNextLocationTask.generated.h"


UCLASS()
class SHOOTER_API UShooterNextLocationTask : public UBTTaskNode
{

	GENERATED_BODY()

public:

	UShooterNextLocationTask(
	);

	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent &OwnerComp,
		uint8 *NodeMemory
	) override;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = 0.0f))
	float LocationSearchRadius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector AimLocationKey;
};
