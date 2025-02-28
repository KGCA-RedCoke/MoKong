// CopyRight KGCA - Team RedCoke


#include "UI/Ability/AbilityBindWidgetBase.h"

#include "Ability/MKAbilitySystemComponent.h"
#include "Ability/MKGameplayEffectUIData.h"

bool UAbilityBindWidgetBase::InitializeAbilitySystemWidget(UAbilitySystemComponent* OwnerAbilitySystemComponent)
{
	AbilitySystemComponent = Cast<UMKAbilitySystemComponent>(OwnerAbilitySystemComponent);

	// The Ability System Component is invalid. Stop here and return false. 
	if (!GetOwnerAbilitySystemComponent())
	{
		return false;
	}

	bool bBindingDone = false;

	if (bListenForEffectEvents && AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->OnMKGameplayEffectEventDelegate.AddDynamic(
																		   this,
																		   &UAbilityBindWidgetBase::EffectChangeCallback);

	}
	bBindingDone = true;

	K2_InitializeAbilitySystemWidget(bBindingDone);

	return bBindingDone;
}

UMKAbilitySystemComponent* UAbilityBindWidgetBase::GetOwnerAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void UAbilityBindWidgetBase::ResetDelegateHandle(FDelegateHandle           DelegateHandle,
												 UAbilitySystemComponent*  OldAbilitySystemComponent,
												 const FGameplayAttribute& Attribute)
{
	if (IsValid(OldAbilitySystemComponent))
	{
		OldAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).Remove(DelegateHandle);
		DelegateHandle.Reset();
	}
}

void UAbilityBindWidgetBase::EffectChangeCallback(const EASEffectEventType EventType, const FActiveGameplayEffect& Effect)
{

	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	FGameplayTagContainer AssetTags{};
	Effect.Spec.GetAllAssetTags(AssetTags);

	if (!EffectEventTagRequirements.RequirementsMet(AssetTags))
	{
		return;
	}

	if (!UMKGameplayEffectUIData::GetGameplayEffectUIDataFromActiveEffect(Effect))
	{
		return;
	}

	FMKEffectEventInfo Info{};
	Info.bIsInhibited = Effect.bIsInhibited;
	Info.Spec         = Effect.Spec;
	Info.Def          = Effect.Spec.Def;
	Info.ActiveEffect = Effect;


	K2_OnGameplayEffectEventCallback(AbilitySystemComponent.Get(), EventType, Effect.Handle, Info);
}
