// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterPauseMenuWidget.generated.h"

class UButton;

UCLASS()
class SHOOTER_API UShooterPauseMenuWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	virtual bool Initialize(
	) override;
	
protected:

	UPROPERTY(meta = (BindWidget))
	UButton *UnpauseButton;

private:

	UFUNCTION()
	void OnUnpause(
	);

};
