// Shooter, All Rights Reserved


#include "UI/ShooterHealthBarWidget.h"

#include "Components/ProgressBar.h"

void UShooterHealthBarWidget::SetHealthPercent(
	float HealthPercent
)
{
	if (!HealthBar) {
		return;
	}

	if (HealthPercent < 0.0f || HealthPercent > 1.0f) {
		return;
	}

	HealthBar->SetVisibility(HealthPercent <= PercentVisibilityThreshold ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	HealthBar->SetFillColorAndOpacity(HealthPercent <= PercentColorThreshold ? InjuredColor : HealthyColor);
	HealthBar->SetPercent(HealthPercent);

}
