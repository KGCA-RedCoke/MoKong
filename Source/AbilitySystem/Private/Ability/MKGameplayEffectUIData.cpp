// CopyRight KGCA - Team RedCoke


#include "Ability/MKGameplayEffectUIData.h"

#include "AbilitySystemComponent.h"
#include "ActiveGameplayEffectHandle.h"

const UMKGameplayEffectUIData* UMKGameplayEffectUIData::GetGameplayEffectUIData(
	FActiveGameplayEffectHandle ActiveEffectHandle)
{
	const UAbilitySystemComponent* const ASC = ActiveEffectHandle.GetOwningAbilitySystemComponent();

	if (!ASC)
	{
		return nullptr;
	}

	const FActiveGameplayEffect* const Effect = ASC->GetActiveGameplayEffect(ActiveEffectHandle);

	if (!Effect)
	{
		return nullptr;
	}

	return GetGameplayEffectUIDataFromActiveEffect(*Effect);
}

const UMKGameplayEffectUIData* UMKGameplayEffectUIData::GetGameplayEffectUIDataFromActiveEffect(
	const FActiveGameplayEffect& ActiveEffect)
{
	const UMKGameplayEffectUIData* const UIData = ActiveEffect.Spec.Def->FindComponent<UMKGameplayEffectUIData>();

	return UIData;
}
