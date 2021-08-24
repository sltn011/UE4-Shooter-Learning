// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterBaseWidget.generated.h"


UCLASS()
class SHOOTER_API UShooterBaseWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	void Show(
	);

protected:

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation *ShowAnimation;
	
};
