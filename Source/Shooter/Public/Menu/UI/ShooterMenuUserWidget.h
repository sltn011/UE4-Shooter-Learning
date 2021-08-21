// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterMenuUserWidget.generated.h"

class UButton;

UCLASS()
class SHOOTER_API UShooterMenuUserWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	virtual bool Initialize(
	) override;

protected:

	UPROPERTY(meta = (BindWidget))
	UButton *StartTestLevelButton;

	UPROPERTY(meta = (BindWidget))
	UButton *StartDustIILevelButton;

private:

	UFUNCTION()
	void OnTestLevelStart(
	);

	UFUNCTION()
	void OnDustIILevelStart(
	);

};
