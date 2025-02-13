// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MKAbilityTask_WaitInput.generated.h"

class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputEventDelegate, FInputActionValue, Value);

/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API UMKAbilityTask_WaitInput : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnInputEventDelegate OnInputEvent;

	UFUNCTION(BlueprintCallable,
		meta = (HidePin = "OwningAbility",
			DefaultToSelf = "OwningAbility",
			BlueprintInternalUseOnly = "TRUE"),
		Category = "Ability|Tasks")
	static UMKAbilityTask_WaitInput* WaitInput(UGameplayAbility* OwningAbility, const FName TaskInstanceName, UInputAction* InputAction, const ETriggerEvent TriggerEventType, bool bShouldOnlyTriggerOnce = true);

private:
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	void EventReceived(const FInputActionValue& Value);

private:
	TWeakObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = nullptr;
	TWeakObjectPtr<UInputAction>            InputAction            = nullptr;

	ETriggerEvent EventType         = ETriggerEvent::Triggered;
	bool          bTriggeredOnce    = false;
	bool          bHasBeenTriggered = false;

};
