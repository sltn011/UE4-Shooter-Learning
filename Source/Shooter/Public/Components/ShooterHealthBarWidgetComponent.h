// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ShooterHealthBarWidgetComponent.generated.h"


UCLASS()
class SHOOTER_API UShooterHealthBarWidgetComponent : public UWidgetComponent
{

	GENERATED_BODY()

public:

	virtual void BeginPlay(
	) override;

	virtual void EndPlay(
		EEndPlayReason::Type const EndPlayReason
	) override;

	UFUNCTION()
	void OnOwnerDeath(
	);

protected:

	void UpdateVisibility(
	);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (ClampMin = 0.1f))
	float RenderDistance = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (ClampMin = 0.1f))
	float VisibilityUpdateTimeFrequency = 1.0f;

private:

	AController *GetObserver(
	) const;

	APawn *GetObserverPawn(
		AController const *Observer
	) const;

	bool GetOwnerLocation(
		FVector &OwnerLocation
	) const;

	bool IsInRenderDistance(
		FVector const &OwnerLocation,
		FVector const &ObserverLocation
	) const;

	bool IsVisibleByObserver(
		FVector const &OwnerLocation,
		FVector const &ObserverLocation
	) const;

	FTimerHandle UpdateTimerHandle;

};
