// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ShooterCoreTypes.h"
#include "ShooterAnimNotify.generated.h"


UCLASS()
class SHOOTER_API UShooterAnimNotify : public UAnimNotify
{

	GENERATED_BODY()
	
public:

	virtual void Notify(
		USkeletalMeshComponent *MeshComp,
		UAnimSequenceBase *Animation
	) override;

	FOnNotified OnNotified;

};
