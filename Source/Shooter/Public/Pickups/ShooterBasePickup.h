// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterBasePickup.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class USoundCue;

UCLASS()
class SHOOTER_API AShooterBasePickup : public AActor
{

	GENERATED_BODY()
	
public:	

	AShooterBasePickup(
	);

	virtual void Tick(
		float DeltaTime
	) override;

	bool IsVisible(
	) const;

protected:

	virtual void BeginPlay(
	) override;

	virtual void NotifyActorBeginOverlap(
		AActor *PickerActor
	) override;

	virtual bool IsPickableCondition(
		AActor *PickerActor
	);

	virtual void PickupEffect(
		APawn *PickerPawn
	);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	USphereComponent *CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Appearance")
	UStaticMeshComponent *MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	bool bIsRotating = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance", meta = (EditCondition = "bIsRotating"))
	bool bIsRandomRotationSpeed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance", meta = (EditCondition = "bIsRotating && !bIsRandomRotationSpeed"))
	float RotationYawSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
	bool bIsRespawnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn", meta = (ClampMin = 0.0f, EditCondition = "bIsRespawnable"))
	float RespawnTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn", meta = (ClampMin = 0.0f, EditCondition = "bIsRespawnable"))
	float DelayBeforePickingAfterRespawn = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue *PickupSound;

private:

	void OnBeingPicked(
	);

	void DisappearFromWorld(
	);

	void ReappearInWorld(
	);

	float GenerateRotationYawSpeed(
	);

	float PickupYawRotation = 0.0f;

	FTimerHandle ReappearTimerHandle;
};
