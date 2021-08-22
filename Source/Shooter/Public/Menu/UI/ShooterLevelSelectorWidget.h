// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterLevelSelectorWidget.generated.h"

class UHorizontalBox;
class UShooterGameInstance;
class UShooterLevelIconWidget;

UCLASS()
class SHOOTER_API UShooterLevelSelectorWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	void SetSelected(
		int32 LevelIndex
	);

	int32 GetSelected(
	) const;

protected:

	virtual void NativeOnInitialized(
	) override;

	UShooterGameInstance *GetGameInstance(
	) const;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox *LevelsIcons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Levels")
	TSubclassOf<UShooterLevelIconWidget> LevelIconClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	float BetweenIconsSpaceWidth = 50.0f;

	int32 CurrentlySelected = 0;
	
};
