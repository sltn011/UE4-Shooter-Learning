// Shooter, All Rights Reserved


#include "Components/ShooterAIWeaponComponent.h"

#include "Weapon/ShooterBaseWeapon.h"

void UShooterAIWeaponComponent::StartShooting(
)
{
	if (!CanShoot()) {
		return;
	}
	
	if (IsAmmoEmpty()) {
		EquipNextWeapon();
	}
	else {
		bIsShooting = true;
		CurrentWeapon->StartShooting();
	}
}

void UShooterAIWeaponComponent::EquipNextWeapon(
)
{
	UE_LOG(LogTemp, Display, TEXT("AI Weapon change!"));
	if (!CanEquip()) {
		return;
	}

	int32 NextWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	while (NextWeaponIndex != CurrentWeaponIndex) {
		if (!Weapons[NextWeaponIndex]->IsAmmoEmpty()) {
			EquipWeapon(NextWeaponIndex);
			return;
		}
		NextWeaponIndex = (NextWeaponIndex + 1) % Weapons.Num();
	}
}