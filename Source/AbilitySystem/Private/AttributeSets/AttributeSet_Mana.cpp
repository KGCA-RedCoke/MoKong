// CopyRight KGCA - Team RedCoke


#include "AttributeSets/AttributeSet_Mana.h"

#include "MKGameplayTags.h"
#include "Net/UnrealNetwork.h"

UAttributeSet_Mana::UAttributeSet_Mana()
	: CurrentMana(2.f),
	  MaximumMana(1.f) {}

void UAttributeSet_Mana::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetCurrentManaAttribute())
	{
		CheckMaxReachedForAttribute(MaximumMana, MoKong::State::TAG_State_Max_Mana.GetTag(), NewValue);
		return;
	}

	if (Attribute == GetMaximumManaAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentManaAttribute(), OldValue, NewValue);
		return;
	}
}

void UAttributeSet_Mana::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributes(Attribute, NewValue);

	if (Attribute == GetCurrentManaAttribute())
	{
		// This should be removed in favor of another method, as we're modifying 2 times the current stamina. (One right before post, one right after (here).
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaximumMana());
		return;
	}

	if (Attribute == GetManaRegenerationAttribute())
	{
		NewValue = FMath::Max(0.f, NewValue);
		return;
	}
}

void UAttributeSet_Mana::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition    = COND_None;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Mana, CurrentMana, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Mana, MaximumMana, Params);

	// Owner Only
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Mana, ManaRegeneration, Params);
}

void UAttributeSet_Mana::OnRep_CurrentMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Mana, CurrentMana, OldValue);

}

void UAttributeSet_Mana::OnRep_MaximumMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Mana, MaximumMana, OldValue);

}

void UAttributeSet_Mana::OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Mana, ManaRegeneration, OldValue);
}
