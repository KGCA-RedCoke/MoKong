// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "MKInputConfig.h"
#include "MKEnhancedInputComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ABILITYSYSTEM_API UMKEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UMKEnhancedInputComponent();

	template <class UserClass, typename FuncType>
	void BindNativeAction(const UMKInputConfig* InputConfig, const FGameplayTag& InputTag,
						  ETriggerEvent         TriggerEvent, UserClass*         Object, FuncType Func);

	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UMKInputConfig* InputConfig, UserClass*       Object, PressedFuncType PressedFunc,
							ReleasedFuncType      ReleasedFunc, TArray<uint32>& BindHandles);
};

template <class UserClass, typename FuncType>
void UMKEnhancedInputComponent::BindNativeAction(const UMKInputConfig* InputConfig, const FGameplayTag& InputTag,
												 ETriggerEvent         TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UMKEnhancedInputComponent::BindAbilityActions(const UMKInputConfig* InputConfig, UserClass*       Object,
												   PressedFuncType       PressedFunc, ReleasedFuncType ReleasedFunc,
												   TArray<uint32>&       BindHandles)
{
	check(InputConfig);
	for (const FMKInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction,
										   ETriggerEvent::Started,
										   Object,
										   PressedFunc,
										   Action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction,
										   ETriggerEvent::Canceled,
										   Object,
										   ReleasedFunc,
										   Action.InputTag).GetHandle());
				BindHandles.Add(BindAction(Action.InputAction,
										   ETriggerEvent::Completed,
										   Object,
										   ReleasedFunc,
										   Action.InputTag).GetHandle());
			}
		}
	}
}
