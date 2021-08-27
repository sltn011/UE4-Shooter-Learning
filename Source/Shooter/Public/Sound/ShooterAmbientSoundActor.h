// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterAmbientSoundActor.generated.h"

class UAudioComponent;

UCLASS()
class SHOOTER_API AShooterAmbientSoundActor : public AActor
{

	GENERATED_BODY()
	
public:	

	AShooterAmbientSoundActor(
	);

protected:

	virtual void BeginPlay(
	) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	UAudioComponent *AudioComponent;

};
