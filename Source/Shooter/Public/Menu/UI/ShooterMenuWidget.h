// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterMenuWidget.generated.h"

class UButton;

UCLASS()
class SHOOTER_API UShooterMenuWidget : public UUserWidget
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

	UPROPERTY(meta = (BindWidget))
	UButton *QuitGameButton;

private:

	UFUNCTION()
	void OnTestLevelStart(
	);

	UFUNCTION()
	void OnDustIILevelStart(
	);

	UFUNCTION()
	void OnQuitGame(
	);

};