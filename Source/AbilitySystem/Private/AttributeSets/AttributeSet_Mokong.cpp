// CopyRight KGCA - Team RedCoke


#include "AttributeSets/AttributeSet_Mokong.h"

#include "Net/UnrealNetwork.h"
UAttributeSet_Mokong::UAttributeSet_Mokong() {}

void UAttributeSet_Mokong::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UAttributeSet_Mokong::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UAttributeSet_Mokong::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UAttributeSet_Mokong::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue,
												   float                     NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UAttributeSet_Mokong::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetCurrentFocusAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetTargetFocus());
	}
}

void UAttributeSet_Mokong::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition    = COND_OwnerOnly;

	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Mokong, Level, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Mokong, CurrentExp, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Mokong, TargetExp, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Mokong, Attack, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Mokong, Defense, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Mokong, CriticalHitChance, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Mokong, CriticalHitDamage, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Mokong, DamageBonus, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Mokong, DamageReduction, Params);
}

void UAttributeSet_Mokong::OnRep_Level(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Mokong, Level, OldValue);
}

void UAttributeSet_Mokong::OnRep_CurrentExp(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Mokong, CurrentExp, OldValue);
}

void UAttributeSet_Mokong::OnRep_TargetExp(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Mokong, TargetExp, OldValue);
}

void UAttributeSet_Mokong::OnRep_Attack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Mokong, Attack, OldValue);
}

void UAttributeSet_Mokong::OnRep_Defense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Mokong, Defense, OldValue);
}

void UAttributeSet_Mokong::OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Mokong, CriticalHitChance, OldValue);
}

void UAttributeSet_Mokong::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Mokong, CriticalHitDamage, OldValue);
}

void UAttributeSet_Mokong::OnRep_DamageBonus(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Mokong, DamageBonus, OldValue);
}

void UAttributeSet_Mokong::OnRep_DamageReduction(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Mokong, DamageReduction, OldValue);
}

void UAttributeSet_Mokong::OnRep_TargetFocus(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Mokong, TargetFocus, OldValue);
}

void UAttributeSet_Mokong::OnRep_CurrentFocus(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Mokong, CurrentFocus, OldValue);
}
