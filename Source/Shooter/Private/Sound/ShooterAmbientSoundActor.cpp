// Shooter, All Rights Reserved


#include "Sound/ShooterAmbientSoundActor.h"

#include "Components/AudioComponent.h"

AShooterAmbientSoundActor::AShooterAmbientSoundActor()
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	SetRootComponent(AudioComponent);
}

void AShooterAmbientSoundActor::BeginPlay()
{
	Super::BeginPlay();
	
	check(AudioComponent);

	AudioComponent->FadeIn(5.0f);
}
