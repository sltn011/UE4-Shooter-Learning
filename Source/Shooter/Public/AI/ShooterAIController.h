// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

class UShooterAIPerceptionComponent;
class UShooterRespawnComponent;

UCLASS()
class SHOOTER_API AShooterAIController : public AAIController
{

	GENERATED_BODY()

public:

	AShooterAIController(
	);

protected:

	virtual void OnPossess(
		APawn *PossessedPawn
	) override;

	virtual void Tick(
		float DeltaTime
	) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Perception")
	UShooterAIPerceptionComponent *AIPerceptionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Perception")
	FName FocusOnActorKeyName = "EnemyActor";

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Respawn")
	UShooterRespawnComponent *RespawnComponent;

private:

	AActor *GetFocusOnActor(
	) const;

};
