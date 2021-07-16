// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterBaseCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UShooterHealthComponent;
class UTextRenderComponent;

class AShooterBaseWeapon;
class UShooterWeaponComponent;


DECLARE_DELEGATE(FOnOwnerDeath);
DECLARE_DELEGATE(FOnOwnerDespawn);


UCLASS()
class SHOOTER_API AShooterBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AShooterBaseCharacter(
		FObjectInitializer const &ObjInitializer
	);

protected:

	virtual void BeginPlay(
	) override;

	virtual void EndPlay(
		EEndPlayReason::Type const EndPlayReason
	) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent *SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent *CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	UShooterHealthComponent *HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	UTextRenderComponent *HealthTextComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	UShooterWeaponComponent *WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage *DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Movement(Fall)")
	FVector2D MinMaxDamagingFallVelocities = FVector2D(900.0f, 1800.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Movement(Fall)")
	FVector2D MinMaxFallDamage = FVector2D(10.0f, 100.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Lifespan")
	float LifeSpanAfterDeath = 5.0f;

public:	

	virtual void Tick(
		float DeltaTime
	) override;

	virtual void SetupPlayerInputComponent(
		class UInputComponent *PlayerInputComponent
	) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsRunning(
	) const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float MoveDirectionRadians(
	) const;

	FOnOwnerDeath   OnOwnerDeath;   // Delegate to tell Actors owned by this character that he died
	FOnOwnerDespawn OnOwnerDespawn; // Delegate to tell Actors owned by this character that he despawned

private:

	void MoveForward(
		float Scale
	);

	void MoveRight(
		float Scale
	);


	void StartRunning(
	);

	void StopRunning(
	);


	void OnHealthChanged(
		float NewHealth
	);
	
	void OnDeath(
	);

	UFUNCTION()
	void OnGroundLanding(
		FHitResult const &HitResult
	);

	bool bIsMovingForward = false;
	bool bIsRunning = false;
};
