// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ShooterHealthPercentDecorator.generated.h"


UCLASS()
class SHOOTER_API UShooterHealthPercentDecorator : public UBTDecorator
{

	GENERATED_BODY()
	
public:

	UShooterHealthPercentDecorator(
	);

protected:

	virtual bool CalculateRawConditionValue(
		UBehaviorTreeComponent &OwnerComp,
		uint8 *NodeMemory
	) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthThreshold = 0.6f;

};
