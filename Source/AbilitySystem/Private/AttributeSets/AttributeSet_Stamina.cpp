// CopyRight KGCA - Team RedCoke


#include "AttributeSets/AttributeSet_Stamina.h"

#include "MKGameplayTags.h"
#include "Net/UnrealNetwork.h"

UAttributeSet_Stamina::UAttributeSet_Stamina()
	: MaximumStamina(1.f),
	  CurrentStamina(1.f),
	  StaminaRegeneration(0.f) {}

void UAttributeSet_Stamina::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetCurrentStaminaAttribute())
	{
		CheckMaxReachedForAttribute(MaximumStamina, MoKong::State::TAG_State_Max_Stamina.GetTag(), NewValue);
		return;
	}

	if (Attribute == GetMaximumStaminaAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentStaminaAttribute(), OldValue, NewValue);
		return;
	}
}

void UAttributeSet_Stamina::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributes(Attribute, NewValue);

	if (Attribute == GetCurrentStaminaAttribute())
	{
		// This should be removed in favor of another method, as we're modifying 2 times the current stamina. (One right before post, one right after (here).
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaximumStamina());
		return;
	}

	if (Attribute == GetStaminaRegenerationAttribute())
	{
		NewValue = FMath::Max(0.f, NewValue);
		return;
	}
}

void UAttributeSet_Stamina::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition    = COND_None;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Stamina, CurrentStamina, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Stamina, MaximumStamina, Params);

	// Owner Only
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Stamina, StaminaRegeneration, Params);
}

void UAttributeSet_Stamina::OnRep_CurrentStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Stamina, CurrentStamina, OldValue);
}

void UAttributeSet_Stamina::OnRep_MaximumStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Stamina, MaximumStamina, OldValue);
}

void UAttributeSet_Stamina::OnRep_StaminaRegeneration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Stamina, StaminaRegeneration, OldValue);
}
