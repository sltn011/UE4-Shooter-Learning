// Shooter, All Rights Reserved


#include "Animations/ShooterAnimNotify.h"

void UShooterAnimNotify::Notify(
	USkeletalMeshComponent *MeshComp,
	UAnimSequenceBase *Animation
)
{
	OnNotified.Broadcast(MeshComp);

	Super::Notify(MeshComp, Animation);
}