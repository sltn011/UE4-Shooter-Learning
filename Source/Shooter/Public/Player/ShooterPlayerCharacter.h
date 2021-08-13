// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/ShooterBaseCharacter.h"
#include "ShooterPlayerCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;


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

private:

	void MoveForward(
		float Scale
	);

	void MoveRight(
		float Scale
	);
	
};
