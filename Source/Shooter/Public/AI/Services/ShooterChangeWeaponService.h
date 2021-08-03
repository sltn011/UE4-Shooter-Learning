// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ShooterChangeWeaponService.generated.h"


UCLASS()
class SHOOTER_API UShooterChangeWeaponService : public UBTService
{

	GENERATED_BODY()

public:

	UShooterChangeWeaponService(
	);

	virtual void TickNode(
		UBehaviorTreeComponent &OwnerComp,
		uint8 *NodeMemory,
		float DeltaSeconds
	) override;

protected:

	bool ShouldChange(
	) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.0f, ClampMax = 1.0f))
	float ChangeWeaponProbability = 0.5f;
};
