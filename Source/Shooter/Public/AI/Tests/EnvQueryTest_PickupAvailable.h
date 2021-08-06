// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_PickupAvailable.generated.h"


UCLASS()
class SHOOTER_API UEnvQueryTest_PickupAvailable : public UEnvQueryTest
{

	GENERATED_BODY()
	
public:

	UEnvQueryTest_PickupAvailable(
		FObjectInitializer const &ObjInitializer
	);

	virtual void RunTest(
		FEnvQueryInstance &QueryInstance
	) const override;

};
