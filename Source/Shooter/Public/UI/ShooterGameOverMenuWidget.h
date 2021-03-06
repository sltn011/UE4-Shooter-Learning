// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/ShooterBaseWidget.h"
#include "ShooterGameOverMenuWidget.generated.h"

class UButton;

UCLASS()
class SHOOTER_API UShooterGameOverMenuWidget : public UShooterBaseWidget
{

	GENERATED_BODY()

public:

	virtual bool Initialize(
	) override;
	
protected:

	UPROPERTY(meta = (BindWidget))
	UButton *RestartLevelButton;

private:

	UFUNCTION()
	void OnRestartLevel(
	);
};
