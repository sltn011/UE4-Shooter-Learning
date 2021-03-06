// Shooter, All Rights Reserved


#include "Components/ShooterCharMovementComponent.h"

#include "Player/ShooterBaseCharacter.h"

UShooterCharMovementComponent::UShooterCharMovementComponent(
)
{
    bRunPhysicsWithNoController = true;
}

float UShooterCharMovementComponent::GetMaxSpeed(
) const
{
    float const MaxSpeed = Super::GetMaxSpeed();
    AShooterBaseCharacter *Character = Cast<AShooterBaseCharacter>(GetPawnOwner());
    if (Character && Character->IsRunning()) {
        return MaxSpeed * RunningSpeedMultiplier;
    }
    return MaxSpeed;
}