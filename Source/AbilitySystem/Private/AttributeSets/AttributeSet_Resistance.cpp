// CopyRight KGCA - Team RedCoke


#include "AttributeSets/AttributeSet_Resistance.h"

#include "Net/UnrealNetwork.h"

UAttributeSet_Resistance::UAttributeSet_Resistance()
	: Resistance(0.f) {}

void UAttributeSet_Resistance::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue,
													   float                     NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UAttributeSet_Resistance::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributes(Attribute, NewValue);

	if (Attribute == GetResistanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, RESISTANCE_MIN, RESISTANCE_MAX);
		return;
	}
}

void UAttributeSet_Resistance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition    = COND_None;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Resistance, Resistance, Params);
}

void UAttributeSet_Resistance::OnRep_Resistance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Resistance, Resistance, OldValue);
}
