// Shooter, All Rights Reserved


#include "Components/ShooterHealthComponent.h"

#include "Camera/CameraShake.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "ShooterGameModeBase.h"
#include "ShooterUtils.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooterHealthComponent, All, All);

UShooterHealthComponent::UShooterHealthComponent(
)
{
	PrimaryComponentTick.bCanEverTick = false;
}


float UShooterHealthComponent::RestoreHealth(
	float HealthRestoredAmount
)
{
	if (HealthRestoredAmount <= 0.0f) {
		return 0.0f;
	}

	float HealthBefore = Health;
	SetHealth(Health + HealthRestoredAmount);

	UE_LOG(LogShooterHealthComponent, Display, TEXT("Health Restore Log: Character: %s, Before: %.3f, After: %.3f, Restored: %.3f"),
		*GetOwner()->GetName(), HealthBefore, Health, Health - HealthBefore
	);

	return Health - HealthBefore;
}

float UShooterHealthComponent::GetHealth(
) const
{
	return Health;
}

float UShooterHealthComponent::GetHealthPercent(
) const
{
	return Health / MaxHealth;
}

bool UShooterHealthComponent::IsDead(
) const
{
	return FMath::IsNearlyZero(Health);
}

void UShooterHealthComponent::BeginPlay(
)
{
	Super::BeginPlay();

	check(MaxHealth > 0.0f);
	check(AutoHealStartDelay >= 0.0f);
	check(AutoHealUpdateTime >= 0.0f);
	check(AutoHealAddedValue >= 0.0f);

	SetHealth(MaxHealth);
	
	AActor *Owner = GetOwner();
	if (Owner) {
		Owner->OnTakeAnyDamage.AddDynamic(this, &UShooterHealthComponent::OnTakeAnyDamage);
	}
	else {
		UE_LOG(LogShooterHealthComponent, Error, TEXT("Error recieving component's Owner!"));
		checkNoEntry();
	}
}

void UShooterHealthComponent::OnTakeAnyDamage(
	AActor *DamagedActor, 
	float Damage, 
	UDamageType const *DamageType, 
	AController *InstigatedBy,
	AActor *DamageCauser
)
{
	if (Damage <= 0 || IsDead() || !GetWorld()) {
		return;
	}

	APawn *DamagedPawn = Cast<APawn>(DamagedActor);
	if (!DamagedPawn) {
		return;
	}

	if (!ShooterUtils::AreEnemies(DamagedPawn->GetController(), InstigatedBy)) {
		return;
	}

	SetHealth(Health - Damage);

	PlayCameraShake(OnDamageCameraShake);

	GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);

	if (IsDead()) {
		RegisterKill(InstigatedBy);
		OnDeath.Broadcast();
	}
	else if (bAutoHeal) {
		if (GetWorld()) {
			GetWorld()->GetTimerManager().SetTimer(AutoHealTimerHandle, this, &UShooterHealthComponent::AutoHeal, AutoHealUpdateTime, true, AutoHealStartDelay);
		}
	}
}

void UShooterHealthComponent::SetHealth(
	float NewHealth
)
{
	NewHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);

	if (FMath::IsNearlyEqual(NewHealth, Health)) {
		return;
	}

	float HealtDelta = NewHealth - Health;
	Health = NewHealth;
	OnHealthChanged.Broadcast(Health, HealtDelta);
}

void UShooterHealthComponent::AutoHeal(
)
{
	SetHealth(Health + AutoHealAddedValue);

	if (FMath::IsNearlyEqual(Health, MaxHealth)) {
		if (GetWorld()) {
			GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
		}
	}
}

void UShooterHealthComponent::PlayCameraShake(
	TSubclassOf<UCameraShakeBase> CameraShake
)
{
	if (!CameraShake && IsDead()) {
		return;
	}

	APawn *OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) {
		return;
	}

	APlayerController *OwnerController = OwnerPawn->GetController<APlayerController>();
	if (!OwnerController || !OwnerController->PlayerCameraManager) {
		return;
	}

	OwnerController->PlayerCameraManager->StartCameraShake(CameraShake);
}

void UShooterHealthComponent::RegisterKill(
	AController *Killer
)
{
	UWorld *World = GetWorld();
	if (!World) {
		return;
	}

	AShooterGameModeBase *GameMode = World->GetAuthGameMode<AShooterGameModeBase>();
	if (!GameMode) {
		return;
	}

	APawn *OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) {
		return;
	}

	AController *Victim = OwnerPawn->GetController();

	GameMode->RegisterKill(Killer, Victim);
}
