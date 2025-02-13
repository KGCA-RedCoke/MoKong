// CopyRight KGCA - Team RedCoke


#include "Input/MKInputConfig.h"

const UInputAction* UMKInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FMKInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."),
		   *InputTag.ToString(), *GetNameSafe(this));

	return nullptr;
}

const UInputAction* UMKInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FMKInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
	UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."),
		   *InputTag.ToString(), *GetNameSafe(this));

	return nullptr;}
