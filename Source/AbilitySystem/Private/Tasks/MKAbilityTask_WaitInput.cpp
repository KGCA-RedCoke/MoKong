// CopyRight KGCA - Team RedCoke


#include "Tasks/MKAbilityTask_WaitInput.h"

UMKAbilityTask_WaitInput* UMKAbilityTask_WaitInput::WaitInput(UGameplayAbility* OwningAbility,
															  const FName TaskInstanceName, UInputAction* InputAction,
															  const ETriggerEvent TriggerEventType,
															  const bool bShouldOnlyTriggerOnce)
{
	UMKAbilityTask_WaitInput* Task_WaitInput = NewAbilityTask<UMKAbilityTask_WaitInput>(OwningAbility, TaskInstanceName);

	Task_WaitInput->InputAction    = InputAction;
	Task_WaitInput->EventType      = TriggerEventType;
	Task_WaitInput->bTriggeredOnce = bShouldOnlyTriggerOnce;

	return Task_WaitInput;
}

void UMKAbilityTask_WaitInput::Activate()
{
	Super::Activate();

	if (!AbilitySystemComponent.Get() || !Ability || !InputAction.IsValid())
		return;

	const APawn* const             Pawn             = Cast<APawn>(Ability->GetAvatarActorFromActorInfo());
	const APlayerController* const PlayerController = Pawn ? Cast<APlayerController>(Pawn->GetController()) : nullptr;

	if (!PlayerController)
	{
		EndTask();
		return;
	}

	// EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->PlayerInput);
	//
	// if (IsValid(EnhancedInputComponent.Get()))
	// {
	// 	EnhancedInputComponent->BindAction(InputAction.Get(), EventType, this, &UMKAbilityTask_WaitInput::EventReceived);
	// }
}

void UMKAbilityTask_WaitInput::OnDestroy(bool bInOwnerFinished)
{
	if (IsValid(EnhancedInputComponent.Get()))
	{
		EnhancedInputComponent->ClearBindingsForObject(this);
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UMKAbilityTask_WaitInput::EventReceived(const FInputActionValue& Value)
{
	if (bTriggeredOnce && bHasBeenTriggered)
		return;

	bHasBeenTriggered = true;

	OnInputEvent.Broadcast(Value);
}
