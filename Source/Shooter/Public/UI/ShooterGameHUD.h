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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Crosshair")
	bool bEnableCrosshair = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Crosshair", meta = (ClampMin = 1, EditCondition = "bEnableCrosshair"))
	int32 Size = 15;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Crosshair", meta = (EditCondition = "bEnableCrosshair"))
	FLinearColor Color = FLinearColor::Green;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Crosshair", meta = (ClampMin = 0.0f, EditCondition = "bEnableCrosshair"))
	float Thickness = 2.0f;

private:

	void DrawCrosshair(
	);

};
