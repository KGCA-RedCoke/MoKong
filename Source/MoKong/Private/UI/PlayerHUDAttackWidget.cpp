// CopyRight KGCA - Team RedCoke


#include "UI/PlayerHUDAttackWidget.h"

#include "MKGameplayTags.h"
#include "Ability/MKAbilitySystemComponent.h"
#include "Ability/MKGameplayEffectUIData.h"
#include "Widgets/EffectWidget_Skill.h"

void UPlayerHUDAttackWidget::EffectChangeCallback(const EASEffectEventType EventType, const FActiveGameplayEffect& Effect)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	FGameplayTagContainer AssetTags{};
	Effect.Spec.GetAllAssetTags(AssetTags);

	// if (!UMKGameplayEffectUIData::GetGameplayEffectUIDataFromActiveEffect(Effect))
	// {
	// 	return;
	// }

	FMKEffectEventInfo Info{};
	Info.bIsInhibited = Effect.bIsInhibited;
	Info.Spec         = Effect.Spec;
	Info.Def          = Effect.Spec.Def;
	Info.ActiveEffect = Effect;

	if (EventType == EASEffectEventType::Added)
	{

		if (AssetTags.HasTag(FGameplayTag::RequestGameplayTag("Cooldown.Immobilize")))
		{
			Slot_1->SetCooldown(Effect.GetTimeRemaining(GetWorld()->GetTimeSeconds()));
		}
		if (AssetTags.HasTag(FGameplayTag::RequestGameplayTag("Cooldown.CloudStep")))
		{
			Slot_2->SetCooldown(Effect.GetTimeRemaining(GetWorld()->GetTimeSeconds()));
		}
		if (AssetTags.HasTag(FGameplayTag::RequestGameplayTag("Cooldown.APluckOfMany")))
		{
			Slot_3->SetCooldown(Effect.GetTimeRemaining(GetWorld()->GetTimeSeconds()));
		}
		if (AssetTags.HasTag(FGameplayTag::RequestGameplayTag("Cooldown.Transformations")))
		{
			Slot_4->SetCooldown(Effect.GetTimeRemaining(GetWorld()->GetTimeSeconds()));
		}
	}
	else
	{}


	K2_OnGameplayEffectEventCallback(AbilitySystemComponent.Get(), EventType, Effect.Handle, Info);
}
