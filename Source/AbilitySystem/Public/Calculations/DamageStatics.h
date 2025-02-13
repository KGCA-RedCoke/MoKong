// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AttributeSets/AttributeSet_Health.h"
#include "AttributeSets/AttributeSet_Resistance.h"
#include "UObject/Object.h"

struct FMKDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Resistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CurrentHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaximumHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ReceivedBleed);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Bleeding);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	FMKDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Resistance, Resistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Health, CurrentHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Health, MaximumHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Health, ReceivedBleed, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Health, Bleeding, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Health, Damage, Target, false);
	}

	static const FMKDamageStatics& DamageStatics()
	{
		static FMKDamageStatics DmgStatics;
		return DmgStatics;
	}
};
