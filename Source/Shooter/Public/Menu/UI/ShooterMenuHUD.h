// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterMenuHUD.generated.h"


UCLASS()
class SHOOTER_API AShooterMenuHUD : public AHUD
{

	GENERATED_BODY()

protected:

	virtual void BeginPlay(
	) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;
	
};
