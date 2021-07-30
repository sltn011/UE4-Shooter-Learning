// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterCoreTypes.h"
#include "ShooterWeaponFXComponent.generated.h"

class UNiagaraSystem;
class UPhysicalMaterial;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UShooterWeaponFXComponent : public UActorComponent
{

	GENERATED_BODY()

public:	

	UShooterWeaponFXComponent(
	);

	void PlayImpactFX(
		FHitResult const &Hit
	);

protected:

	virtual void BeginPlay(
	) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FImpactData DefaultImpactData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TMap<UPhysicalMaterial *, FImpactData> ImpactDataMap;

};
