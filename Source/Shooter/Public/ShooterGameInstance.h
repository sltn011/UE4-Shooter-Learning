// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ShooterCoreTypes.h"
#include "ShooterGameInstance.generated.h"


UCLASS()
class SHOOTER_API UShooterGameInstance : public UGameInstance
{

	GENERATED_BODY()

public:

	virtual void Init(
	) override;

	FName GetMenuLevelName(
	) const;

	FName GetTestLevelName(
	) const;

	FName GetDustIILevelName(
	) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Levels")
	TArray<FLevelData> Levels;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Levels")
	FName MenuLevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Levels")
	FName TestLevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Levels")
	FName DustIILevelName = NAME_None;
	
};
