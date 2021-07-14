// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterWeaponComponent.generated.h"

class AShooterBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UShooterWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UShooterWeaponComponent(
	);

	void Shoot(
	);

protected:

	virtual void BeginPlay(
	) override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AShooterBaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponAttachSocketName = TEXT("WeaponSocket");

private:

	UPROPERTY()
	AShooterBaseWeapon *CurrentWeapon = nullptr;

	void SpawnWeapon(
	);

};
