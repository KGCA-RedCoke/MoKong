// CopyRight KGCA - Team RedCoke


#include "HealthBar/CatchUpBarBase.h"

#include "Components/ProgressBar.h"

void UCatchUpBarBase::NativeConstruct()
{
	Super::NativeConstruct();

	OnMainPercentChanged.AddUniqueDynamic(this, &UCatchUpBarBase::UpdateMainPercent);
	OnCatchupPercentChanged.AddUniqueDynamic(this, &UCatchUpBarBase::UpdateCatchupPercent);

	MainProgressBar->SetFillColorAndOpacity(MainColor);
	CatchupProgressBar->SetFillColorAndOpacity(CatchupColor);
}

void UCatchUpBarBase::InitializeCatchupValues(float InCurrentValue, float InMaxValue)
{
	// SetMaxValue(InMaxValue);
	// SetCurrentValue(InCurrentValue, TODO);
}

void UCatchUpBarBase::SetCurrentValue(float NewValue, float OldValue)
{
	const float Delta = NewValue - OldValue;

	CurrentValue = NewValue;

	ReCalculatePercent(Delta);
}

void UCatchUpBarBase::SetMaxValue(float NewValue, float OldValue)
{
	const float Delta = NewValue - OldValue;

	MaxValue = NewValue;

	ReCalculatePercent(Delta);
}

void UCatchUpBarBase::ReCalculatePercent(float Delta)
{
	float NewPercent = FMath::Clamp(CurrentValue / MaxValue, 0.f, 1.f);

	if (!FMath::IsNearlyZero(Delta))
	{
		MainPercent = NewPercent;

		MainProgressBar->SetPercent(MainPercent);
	}
}

void UCatchUpBarBase::SetCatchupPercent(float NewPercent)
{
	if (!FMath::IsNearlyEqual(NewPercent, CatchupPercent))
	{
		CatchupPercent = NewPercent;

		OnCatchupPercentChanged.Broadcast(CatchupPercent);
	}
}

void UCatchUpBarBase::StartCatchingUp()
{
	State = ECatchupBarState::CatchingUp;
}

void UCatchUpBarBase::UpdateMainPercent(float NewPercent)
{}

void UCatchUpBarBase::UpdateCatchupPercent(float NewPercent)
{}
