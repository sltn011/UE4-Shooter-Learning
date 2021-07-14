// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterDevDamageActor.generated.h"

class USceneComponent;

UCLASS()
class SHOOTER_API AShooterDevDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AShooterDevDamageActor(
	);

protected:

	virtual void BeginPlay(
	) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent *SceneComponent;

	UPROPERTY(EditAnywhere, Category = "Visual")
	float Radius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Visual")
	FColor SphereColor = FColor::Red;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float Damage = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	bool bDoFullDamage = false;

	UPROPERTY(EditAnywhere, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;

public:	

	virtual void Tick(
		float DeltaTime
	) override;

};
