// Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ShooterGameInstance.generated.h"


UCLASS()
class SHOOTER_API UShooterGameInstance : public UGameInstance
{

	GENERATED_BODY()

public:

	FName GetTestLevelName(
	) const;

	FName GetDustIILevelName(
	) const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Levels")
	FName TestLevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Levels")
	FName DustIILevelName = NAME_None;
	
};
