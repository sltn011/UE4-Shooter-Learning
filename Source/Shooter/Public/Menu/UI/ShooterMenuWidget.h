// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterMenuWidget.generated.h"

class UButton;
class UShooterLevelSelectorWidget;

UCLASS()
class SHOOTER_API UShooterMenuWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	virtual bool Initialize(
	) override;

protected:

	UPROPERTY(meta = (BindWidget))
	UShooterLevelSelectorWidget *LevelSelector;

	UPROPERTY(meta = (BindWidget))
	UButton *PlaySelectedLevelButton;

	UPROPERTY(meta = (BindWidget))
	UButton *QuitGameButton;

private:

	UFUNCTION()
	void OnPlaySelectedLevel(
	);

	UFUNCTION()
	void OnQuitGame(
	);

};
