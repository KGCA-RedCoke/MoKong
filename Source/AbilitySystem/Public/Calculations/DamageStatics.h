// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AttributeSets/AttributeSet_Health.h"
#include "AttributeSets/AttributeSet_Mokong.h"
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
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	FMKDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Resistance, Resistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Health, CurrentHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Health, MaximumHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Health, ReceivedBleed, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Health, Bleeding, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Health, Damage, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Mokong, Attack, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Mokong, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSet_Mokong, CriticalHitDamage, Source, false);
	}

	static const FMKDamageStatics& DamageStatics()
	{
		static FMKDamageStatics DmgStatics;
		return DmgStatics;
	}
};
