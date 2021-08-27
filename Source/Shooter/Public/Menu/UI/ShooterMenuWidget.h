// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/ShooterBaseWidget.h"
#include "ShooterMenuWidget.generated.h"

class UButton;
class UShooterLevelSelectorWidget;
class USoundCue;

UCLASS()
class SHOOTER_API UShooterMenuWidget : public UShooterBaseWidget
{

	GENERATED_BODY()

public:

	virtual bool Initialize(
	) override;

protected:

	virtual void OnAnimationFinished_Implementation(
		UWidgetAnimation const *Animation
	) override;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation *LoadingAnimation;

	UPROPERTY(meta = (BindWidget))
	UShooterLevelSelectorWidget *LevelSelector;

	UPROPERTY(meta = (BindWidget))
	UButton *PlaySelectedLevelButton;

	UPROPERTY(meta = (BindWidget))
	UButton *QuitGameButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue *StartGameSound;

private:

	void LoadAndPlaySelectedLevel(
	);

	UFUNCTION()
	void OnPlaySelectedLevel(
	);

	UFUNCTION()
	void OnQuitGame(
	);

};
