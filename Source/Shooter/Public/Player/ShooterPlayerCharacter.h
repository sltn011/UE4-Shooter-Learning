// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/ShooterBaseCharacter.h"
#include "ShooterPlayerCharacter.generated.h"


class UCameraComponent;
class USphereComponent;
class USpringArmComponent;


UCLASS()
class SHOOTER_API AShooterPlayerCharacter : public AShooterBaseCharacter
{

	GENERATED_BODY()

public:

	AShooterPlayerCharacter(
		FObjectInitializer const &ObjInitializer
	);

	virtual void SetupPlayerInputComponent(
		class UInputComponent *PlayerInputComponent
	) override;

protected:

	virtual void BeginPlay(
	) override;

	virtual void OnDeath(
	) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent *SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent *CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USphereComponent *CameraCollisionComponent;

private:

	void MoveForward(
		float Scale
	);

	void MoveRight(
		float Scale
	);

	UFUNCTION()
	void OnCameraBeginOverlap(
		UPrimitiveComponent *OverlappedComponent,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComp, 
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult
	);

	UFUNCTION()
	void OnCameraEndOverlap(
		UPrimitiveComponent *OverlappedComponent,
		AActor *OtherActor,
		UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex
	);

	void CameraOverlapHandle(
	);
	
};
