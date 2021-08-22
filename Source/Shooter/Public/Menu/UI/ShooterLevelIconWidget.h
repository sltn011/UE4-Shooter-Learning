// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShooterCoreTypes.h"
#include "ShooterLevelIconWidget.generated.h"

class UButton;
class UShooterGameInstance;
class UShooterLevelSelectorWidget;

UCLASS()
class SHOOTER_API UShooterLevelIconWidget : public UUserWidget
{

	GENERATED_BODY()
	
public:

	virtual bool Initialize(
	) override;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetLevelData(
		FLevelData &LevelData
	) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool IsSelected(
	) const;

	UShooterLevelSelectorWidget *ParentWidget = nullptr;

	int32 LevelIndex = 0;

protected:

	UFUNCTION()
	void OnLevelSelected(
	);

	UPROPERTY(meta = (BindWidget))
	UButton *SelectButton;

};
