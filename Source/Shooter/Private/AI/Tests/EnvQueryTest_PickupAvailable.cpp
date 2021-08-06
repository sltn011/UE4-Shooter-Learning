// Shooter, All Rights Reserved


#include "AI/Tests/EnvQueryTest_PickupAvailable.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "Pickups/ShooterBasePickup.h"

UEnvQueryTest_PickupAvailable::UEnvQueryTest_PickupAvailable(
	FObjectInitializer const &ObjInitializer
)
	: Super{ ObjInitializer }
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
	SetWorkOnFloatValues(false);
}

void UEnvQueryTest_PickupAvailable::RunTest(
	FEnvQueryInstance &QueryInstance
) const
{
	UObject *DataOwner = QueryInstance.Owner.Get();
	BoolValue.BindData(DataOwner, QueryInstance.QueryID);

	bool bWantsBeTakeable = BoolValue.GetValue();

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		AActor *ItemActor = GetItemActor(QueryInstance, It.GetIndex());
		AShooterBasePickup *Pickup = Cast<AShooterBasePickup>(ItemActor);
		if (!Pickup) {
			continue;
		}

		bool bIsEQSVisible = Pickup->IsVisible();

		It.SetScore(TestPurpose, FilterType, bIsEQSVisible, bWantsBeTakeable);
	}
}
