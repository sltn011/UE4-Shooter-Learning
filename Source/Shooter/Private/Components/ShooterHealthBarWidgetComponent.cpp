// Shooter, All Rights Reserved


#include "Components/ShooterHealthBarWidgetComponent.h"

#include "TimerManager.h"

void UShooterHealthBarWidgetComponent::BeginPlay(
)
{
    Super::BeginPlay();

    UWorld *World = GetWorld();
    if (!World) {
        return;
    }

    World->GetTimerManager().SetTimer(
        UpdateTimerHandle,
        this,
        &UShooterHealthBarWidgetComponent::UpdateVisibility,
        VisibilityUpdateTimeFrequency,
        true
    );
}

void UShooterHealthBarWidgetComponent::EndPlay(
    EEndPlayReason::Type const EndPlayReason
)
{
    OnOwnerDeath();

    Super::EndPlay(EndPlayReason);
}

void UShooterHealthBarWidgetComponent::OnOwnerDeath(
)
{
    UWorld *World = GetWorld();
    if (!World) {
        return;
    }

    World->GetTimerManager().ClearTimer(UpdateTimerHandle);
}

void UShooterHealthBarWidgetComponent::UpdateVisibility(
)
{
    UWorld *World = GetWorld();
    if (!World) {
        return;
    }

    AController *Observer = World->GetFirstPlayerController<AController>();
    APawn *ObserverPawn = GetObserverPawn(Observer);
    if (!ObserverPawn) {
        return;
    }

    FVector ObserverLocation = ObserverPawn->GetActorLocation();
    FVector OwnerLocation;
    if (!GetOwnerLocation(OwnerLocation)) {
        return;
    }

    if (!IsInRenderDistance(OwnerLocation, ObserverLocation)) {
        SetVisibility(false, true);
    }
    else {
        SetVisibility(IsVisibleByObserver(OwnerLocation, ObserverLocation));
    }
}

AController *UShooterHealthBarWidgetComponent::GetObserver(
) const
{
    return GetWorld() ? GetWorld()->GetFirstPlayerController<AController>() : nullptr;
}

APawn *UShooterHealthBarWidgetComponent::GetObserverPawn(
    AController const *Observer
) const
{
    APlayerController const *PlayerObserver = Cast<APlayerController>(Observer);
    if (!PlayerObserver) {
        return nullptr;
    }

    return PlayerObserver->GetPawnOrSpectator();
}

bool UShooterHealthBarWidgetComponent::GetOwnerLocation(
    FVector &OwnerLocation
) const
{
    APawn *OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn) {
        return false;
    }

    OwnerLocation = OwnerPawn->GetActorLocation();
    return true;
}

bool UShooterHealthBarWidgetComponent::IsInRenderDistance(
    FVector const &OwnerLocation,
    FVector const &ObserverLocation
) const
{
    return FVector::DistSquared(OwnerLocation, ObserverLocation) <= RenderDistance * RenderDistance;
}

bool UShooterHealthBarWidgetComponent::IsVisibleByObserver(
    FVector const &OwnerLocation,
    FVector const &ObserverLocation
) const
{
    UWorld *World = GetWorld();
    if (!World) {
        return false;
    }

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetOwner());
    QueryParams.AddIgnoredActor(GetObserverPawn(GetObserver()));

    FHitResult HitResult;
    return !World->LineTraceSingleByChannel(HitResult, ObserverLocation, OwnerLocation, ECC_Visibility, QueryParams);
}
