// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "ShooterEnemyEnvQueryContext.generated.h"


UCLASS()
class SHOOTER_API UShooterEnemyEnvQueryContext : public UEnvQueryContext
{

	GENERATED_BODY()

public:

	virtual void ProvideContext(
		FEnvQueryInstance &QueryInstance,
		FEnvQueryContextData &ContextData
	) const override;

protected:

	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = "Enemy")
	FName EnemyActorKeyName = "EnemyActor";
	
};
