// CopyRight KGCA - Team RedCoke


#include "AttributeSets/AttributeSet_Health.h"
#include "GameplayEffectExtension.h"
#include "MKGameplayTags.h"
#include "Actor/MKAbilityCharacter.h"

#include "Data/MKAbilitySystemData.h"
#include "Net/UnrealNetwork.h"

UAttributeSet_Health::UAttributeSet_Health()
	: CurrentHealth(1.f),
	  MaximumHealth(1.f),
	  HealthRegeneration(0.f),
	  Bleeding(0.f),
	  BleedHealing(0.f) {}

void UAttributeSet_Health::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (const FMKAttributeMaxValue* MaxValue = AttributeMaxValue.Find(Attribute))
	{
		const float Max = MaxValue->MaxAttribute.IsValid()
							  ? MaxValue->MaxAttribute.GetNumericValue(this)
							  : MaxValue->MaxFloat.GetValueAtLevel(0);

		NewValue = FMath::Clamp(NewValue, 0.f, Max);
		return;
	}

}

void UAttributeSet_Health::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	AMKAbilityCharacter* TargetCharacter = Cast<AMKAbilityCharacter>(Data.Target.AbilityActorInfo->AvatarActor.Get());


	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Store a local copy of the amount of Damage done and clear the Damage attribute.
		const float LocalDamageDone = GetDamage();

		SetDamage(0.f);

		if (LocalDamageDone > 0.0f)
		{
			// Apply the Health change and then clamp it.
			const float NewHealth = GetCurrentHealth() - LocalDamageDone;

			SetCurrentHealth(FMath::Clamp(NewHealth, 0.0f, GetMaximumHealth()));

			const FHitResult* Hit = Data.EffectSpec.GetContext().GetHitResult();

			// if (Hit)
			// {
			// 	EHitReactDirection Direction = EHitReactDirection::Forward;
			//
			// 	TargetCharacter->PlayHitReact(Direction);
			// }
		}
		return;
	}

	if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		// Store a local copy of the amount of Healing done and clear the Healing attribute.
		const float LocalHealingDone = GetHealing();

		SetHealing(0.f);

		if (LocalHealingDone > 0.0f)
		{
			// Apply the Health change and then clamp it.
			const float NewHealth = GetCurrentHealth() + LocalHealingDone;

			SetCurrentHealth(FMath::Clamp(NewHealth, 0.0f, GetMaximumHealth()));
		}
		return;
	}

	if (Data.EvaluatedData.Attribute == GetHealthRegenerationAttribute())
	{
		SetHealthRegeneration(FMath::Clamp(GetHealthRegeneration(), 0.0f, GetMaximumHealth()));
		return;
	}
}

void UAttributeSet_Health::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetCurrentHealthAttribute())
	{
		CheckMaxReachedForAttribute(MaximumHealth, MoKong::State::TAG_State_Max_Health.GetTag(), NewValue);
		return;
	}

	if (Attribute == GetBleedingAttribute())
	{
		CheckStatusTagForAttribute(MoKong::Statuses::TAG_Debuff_Bleeding, NewValue, OldValue);
		return;
	}

	if (Attribute == GetMaximumHealthAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentHealthAttribute(), OldValue, NewValue);
		return;
	}
}

void UAttributeSet_Health::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue,
												   float                     NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UAttributeSet_Health::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetMaximumHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
		return;
	}
	if (Attribute == GetCurrentHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaximumHealth());
		return;
	}
	if (Attribute == GetBleedingAttribute() || Attribute == GetBleedHealingAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
		return;
	}
}

void UAttributeSet_Health::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition    = COND_None;

	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Health, CurrentHealth, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Health, MaximumHealth, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Health, Bleeding, Params);

	// Only Owner
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Health, HealthRegeneration, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(UAttributeSet_Health, BleedHealing, Params);
}

void UAttributeSet_Health::OnRep_CurrentHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Health, CurrentHealth, OldValue);
}

void UAttributeSet_Health::OnRep_MaximumHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Health, MaximumHealth, OldValue);
}

void UAttributeSet_Health::OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Health, HealthRegeneration, OldValue);

}

void UAttributeSet_Health::OnRep_Bleeding(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Health, Bleeding, OldValue);

}

void UAttributeSet_Health::OnRep_BleedHealing(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Health, BleedHealing, OldValue);
}
