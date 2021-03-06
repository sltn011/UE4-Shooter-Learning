// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterBaseCharacter.generated.h"


class AShooterBaseWeapon;
class UShooterHealthComponent;
class UShooterHealthBarWidgetComponent;
class UShooterWeaponComponent;
class USoundCue;


UCLASS()
class SHOOTER_API AShooterBaseCharacter : public ACharacter
{

	GENERATED_BODY()

public:

	AShooterBaseCharacter(
		FObjectInitializer const &ObjInitializer
	);

	void StartRunning(
	);

	void StopRunning(
	);

	void StartShooting(
	);

	void StopShooting(
	);

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead(
	) const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsRunning(
	) const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float MoveDirectionRadians(
	) const;

	void SetPlayerColor(
		FLinearColor const &Color
	);

	virtual void TurnOff(
	) override;

	virtual void Reset(
	) override;

protected:

	virtual void BeginPlay(
	) override;

	virtual void OnDeath(
	);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	UShooterHealthComponent *HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	UShooterHealthBarWidgetComponent *HealthBarComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	UShooterWeaponComponent *WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Movement(Fall)")
	FVector2D MinMaxDamagingFallVelocities = FVector2D(900.0f, 1800.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Movement(Fall)")
	FVector2D MinMaxFallDamage = FVector2D(10.0f, 100.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Lifespan", meta = (ClampMin = 0.0f))
	float LifeSpanAfterDeath = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	FName MaterialColorName = "Paint Color";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue *DeathSound;

	bool bIsMovingForward = false;

	bool bIsRunning = false;

private:

	void OnHealthChanged(
		float NewHealth,
		float HealthDelta
	);

	UFUNCTION()
	void OnGroundLanding(
		FHitResult const &HitResult
	);

	virtual void FellOutOfWorld(
		UDamageType const &DmgType
	) override;

};
