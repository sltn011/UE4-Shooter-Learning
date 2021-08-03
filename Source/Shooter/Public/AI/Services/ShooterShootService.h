// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ShooterShootService.generated.h"


UCLASS()
class SHOOTER_API UShooterShootService : public UBTService
{

	GENERATED_BODY()

public:

	UShooterShootService(
	);

protected:

	virtual void TickNode(
		UBehaviorTreeComponent &OwnerComp,
		uint8 *NodeMemory,
		float DeltaSeconds
	) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	FBlackboardKeySelector TargetActorKey;
	
};
