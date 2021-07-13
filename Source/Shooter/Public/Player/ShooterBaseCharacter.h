// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterBaseCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UShooterHealthComponent;
class UTextRenderComponent;


UCLASS()
class SHOOTER_API AShooterBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterBaseCharacter(
		FObjectInitializer const &ObjInitializer
	);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay(
	) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent *SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent *CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UShooterHealthComponent *HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTextRenderComponent *HealthTextComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage *DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D MinMaxDamagingFallVelocities = FVector2D(900.0f, 1800.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D MinMaxFallDamage = FVector2D(10.0f, 100.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float LifeSpanAfterDeath = 5.0f;

public:	
	// Called every frame
	virtual void Tick(
		float DeltaTime
	) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(
		class UInputComponent *PlayerInputComponent
	) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsRunning(
	) const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float MoveDirectionRadians(
	) const;

private:

	void MoveForward(
		float Scale
	);

	void MoveRight(
		float Scale
	);

	bool bIsMovingForward = false;


	void StartRunning(
	);

	void StopRunning(
	);

	bool bIsRunning = false;


	void OnHealthChanged(
		float NewHealth
	);
	
	void OnDeath(
	);

	UFUNCTION()
	void OnGroundLanding(FHitResult const &HitResult);

};
