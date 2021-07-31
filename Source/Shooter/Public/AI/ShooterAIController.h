// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

class UBehaviorTree;

UCLASS()
class SHOOTER_API AShooterAIController : public AAIController
{

	GENERATED_BODY()

public:

	virtual void BeginPlay(
	) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree *BehaviorTree;
	
};
