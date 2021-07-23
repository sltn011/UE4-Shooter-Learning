// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterGameHUD.generated.h"

UCLASS()
class SHOOTER_API AShooterGameHUD : public AHUD
{

	GENERATED_BODY()
	
public:

	virtual void DrawHUD(
	) override;

protected:

	virtual void BeginPlay(
	) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

};
