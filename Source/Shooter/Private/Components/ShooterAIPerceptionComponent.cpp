// Shooter, All Rights Reserved


#include "Components/ShooterAIPerceptionComponent.h"

#include "AIController.h"
#include "Components/ShooterHealthComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"
#include "ShooterUtils.h"


AActor *UShooterAIPerceptionComponent::GetClosestVisibleEnemyActor(
) const
{
    TArray<AActor *> PerceivedActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
    if (PerceivedActors.Num() == 0) { // If no one in sight 

        GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PerceivedActors); // Check if anyone damaged us
        if (PerceivedActors.Num() == 0) {
            return nullptr;
        }

    }

    AAIController const *Controller = Cast<AAIController>(GetOwner());
    if (!Controller) {
        return nullptr;
    }

    APawn const *Pawn = Controller->GetPawn();
    if (!Pawn) {
        return nullptr;
    }

    float ClosestDistanceSqr = MAX_FLT;
    AActor *ClosestActor = nullptr;
    for (AActor *PerceivedActor : PerceivedActors) {

        UShooterHealthComponent *HealthComponent = ShooterUtils::GetPlayerComponentByClass<UShooterHealthComponent>(PerceivedActor);
        if (!HealthComponent || HealthComponent->IsDead()) {
            continue;
        }

        APawn *PerceivedPawn = Cast<APawn>(PerceivedActor);
        if (!PerceivedPawn) {
            continue;
        }

        if (!ShooterUtils::AreEnemies(Pawn->GetController(), PerceivedPawn->GetController())) {
            continue;
        }

        float DistSqr = FVector{ Pawn->GetActorLocation() - PerceivedActor->GetActorLocation() }.SizeSquared();

        if (DistSqr < ClosestDistanceSqr) {
            ClosestDistanceSqr = DistSqr;
            ClosestActor = PerceivedActor;
        }
    }

    return ClosestActor;
}