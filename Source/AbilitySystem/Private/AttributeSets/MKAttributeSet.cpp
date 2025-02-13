// CopyRight KGCA - Team RedCoke


#include "AttributeSets/MKAttributeSet.h"

#include "AbilitySystemComponent.h"

void UMKAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttribute& AffectedAttribute, const float OldMaxValue,
												  const float               NewMaxValue) const
{
	UAbilitySystemComponent* const AbilityComp = GetOwningAbilitySystemComponent();
	if (!AbilityComp)
	{
		return;
	}

	if (OldMaxValue <= 0.f || FMath::IsNearlyEqual(OldMaxValue, NewMaxValue, 0.f))
	{
		return;
	}

	AbilityComp->SetNumericAttributeBase(AffectedAttribute,
										 AbilityComp->GetNumericAttributeBase(AffectedAttribute) * NewMaxValue /
										 OldMaxValue);
}

void UMKAttributeSet::CheckMaxReachedForAttribute(const FGameplayAttributeData& MaxAttribute, const FGameplayTag& MaxTag,
												  const float&                  NewValue) const
{
	UAbilitySystemComponent* const AbilityComp = GetOwningAbilitySystemComponent();
	if (!AbilityComp)
	{
		return;
	}

	const float Max     = MaxAttribute.GetCurrentValue();
	const bool  bHasTag = AbilityComp->HasMatchingGameplayTag(MaxTag);

	int32 Count = -1;
	if (NewValue < Max && bHasTag)
	{
		Count = 0;
	}
	else if (NewValue >= Max && bHasTag)
	{
		Count = 1;
	}

	if (Count >= 0)
	{
		AbilityComp->SetLooseGameplayTagCount(MaxTag, Count);
		AbilityComp->SetReplicatedLooseGameplayTagCount(MaxTag, Count);
	}
}

void UMKAttributeSet::CheckStatusTagForAttribute(const FGameplayTag& StatusTag, const float& NewValue,
												 const float&        OldValue) const
{
	UAbilitySystemComponent* const AbilityComp = GetOwningAbilitySystemComponent();
	if (!AbilityComp)
	{
		return;
	}

	int32 Count = -1;

	if (NewValue <= 0.f && OldValue > 0.f)
	{
		Count = 0;
	}
	else if (NewValue > 0.f && OldValue <= 0.f)
	{
		Count = 1;
	}

	if (Count >= 0)
	{
		AbilityComp->SetLooseGameplayTagCount(StatusTag, Count);
		AbilityComp->SetReplicatedLooseGameplayTagCount(StatusTag, Count);
	}
}

void UMKAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	ClampAttributes(Attribute, NewValue);
}

void UMKAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	ClampAttributes(Attribute, NewValue);
}
