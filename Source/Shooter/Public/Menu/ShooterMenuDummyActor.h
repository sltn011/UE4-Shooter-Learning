// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterMenuDummyActor.generated.h"

class AShooterBaseWeapon;
class USkeletalMeshComponent;

UCLASS()
class SHOOTER_API AShooterMenuDummyActor : public AActor
{

	GENERATED_BODY()
	
public:	

	AShooterMenuDummyActor(
	);

protected:

	virtual void BeginPlay(
	) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent *SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AShooterBaseWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponSocketName = "WeaponSocket";

};
