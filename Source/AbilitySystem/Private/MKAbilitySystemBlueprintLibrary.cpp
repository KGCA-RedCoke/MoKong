// CopyRight KGCA - Team RedCoke


#include "MKAbilitySystemBlueprintLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AttributeSets/AttributeSet_Resistance.h"
#include "Data/MKAbilitySystemData.h"

float UMKAbilitySystemBlueprintLibrary::GetAttributeValueFromActor(const AActor* const        Actor,
																   const FGameplayAttribute   Attribute,
																   const EAttributeSearchType SearchType)
{
	const UAbilitySystemComponent* const AbilitySystemComponent =
			UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);

	if (!AbilitySystemComponent)
	{
		return -1.f;
	}

	float ReturnValue = -1.0f;
	GetAttributeValue(AbilitySystemComponent, Attribute, SearchType, ReturnValue);

	return ReturnValue;
}

float UMKAbilitySystemBlueprintLibrary::GetAttributeValueFromAbilitySystem(
	const UAbilitySystemComponent* const AbilitySystemComponent, const FGameplayAttribute Attribute,
	const EAttributeSearchType           SearchType)
{
	float ReturnValue = -1.0f;

	GetAttributeValue(AbilitySystemComponent, Attribute, SearchType, ReturnValue);

	return ReturnValue;
}

float UMKAbilitySystemBlueprintLibrary::CalculateEffectiveResistance(const float CurrentArmor)
{
	const float CurrentArmorTmp = FMath::Clamp(CurrentArmor,
											   UAttributeSet_Resistance::RESISTANCE_MIN,
											   UAttributeSet_Resistance::RESISTANCE_MAX);

	const float ArmorDenominator = UAttributeSet_Resistance::RESISTANCE_BASE + CurrentArmorTmp;
	return UAttributeSet_Resistance::RESISTANCE_BASE > 0.f && ArmorDenominator > 0.f
			   ? UAttributeSet_Resistance::RESISTANCE_BASE / ArmorDenominator
			   : 1.f;
}

void UMKAbilitySystemBlueprintLibrary::GetAttributeValue(const UAbilitySystemComponent* const AbilitySystemComponent,
														 const FGameplayAttribute& Attribute,
														 const EAttributeSearchType SearchType, float& ReturnValue)
{
	ReturnValue = -1.0f;

	if (!AbilitySystemComponent || !AbilitySystemComponent->HasAttributeSetForAttribute(Attribute))
	{
		return;
	}

	switch (SearchType)
	{
	case EAttributeSearchType::FinalValue:
		{
			ReturnValue = AbilitySystemComponent->GetNumericAttribute(Attribute);

			return;
		}

	case EAttributeSearchType::BaseValue:
		{
			ReturnValue = AbilitySystemComponent->GetNumericAttributeBase(Attribute);

			return;
		}

	case EAttributeSearchType::BonusValue:
		{
			ReturnValue = AbilitySystemComponent->GetNumericAttribute(Attribute) - AbilitySystemComponent->
					GetNumericAttributeBase(Attribute);
		}
	}
}
