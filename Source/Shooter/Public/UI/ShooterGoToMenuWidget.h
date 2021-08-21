// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterGoToMenuWidget.generated.h"

class UButton;

UCLASS()
class SHOOTER_API UShooterGoToMenuWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	virtual bool Initialize(
	) override;

protected:

	UFUNCTION()
	void OnGoToMenuButtonClicked(
	);

	UPROPERTY(meta = (BindWidget))
	UButton *GoToMenuButton;
	
};
