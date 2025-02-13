// CopyRight KGCA - Team RedCoke


#include "Ability/MKAbilitySystemComponent.h"

#include "AbilitySystemGlobals.h"
#include "Ability/MKGameplayAbility.h"
#include "Actor/MKAbilityCharacter.h"

UMKAbilitySystemComponent::UMKAbilitySystemComponent()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();

	// Sets the Ability System Component to use "Mixed" replication mode. This will replicate minimal Gameplay Effects to Simulated Proxies and full info to everyone else.
	ReplicationMode = EGameplayEffectReplicationMode::Mixed;

	// Explicitly set the Ability System Component to replicate.
	SetIsReplicatedByDefault(true);
}

void UMKAbilitySystemComponent::InitializeAbilitySystemData(
	const FAbilitySystemInitializationData& AbilitySystemInitializationData, AActor* InOwningActor,
	AActor*                                 InAvatarActor)
{
	if (AbilitySystemDataInitialized)
	{
		return;
	}

	AbilitySystemDataInitialized = true;

	// Set the Owning Actor and Avatar Actor. (Used throughout the Gameplay Ability System to get references etc.)
	InitAbilityActorInfo(InOwningActor, InAvatarActor);

	// Apply the Gameplay Tag container as loose Gameplay Tags. (These are not replicated by default and should be applied on both server and client respectively.)
	if (!AbilitySystemInitializationData.GameplayTags.IsEmpty())
	{
		AddLooseGameplayTags(AbilitySystemInitializationData.GameplayTags);
	}

	/** This is our entry point for other component to react to gameplay effect added and removed.
	 * Changes to the gameplay effects are handled differently (using their own delegate sets)
	 * -> FActiveGameplayEffectEvents* EventSet = ASC->GetActiveEffectEventSet(Handle);
	 * (See EffectWidgetControllerBase::InitializeController_Implementation)
	 */
	OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this,
														 &UMKAbilitySystemComponent::OnGameplayEffectAddedCallback);
	ActiveGameplayEffects.OnActiveGameplayEffectRemovedDelegate.AddUObject(this,
																		   &UMKAbilitySystemComponent::OnGameplayEffectRemovedCallback);

	// Check to see if we have authority. (Attribute Sets / Attribute Base Values / Gameplay Abilities / Gameplay Effects should only be added -or- set on authority and will be replicated to the client automatically.)
	if (!GetOwnerActor()->HasAuthority())
	{
		return;
	}

	// Grant Attribute Sets if the array isn't empty.
	if (!AbilitySystemInitializationData.AttributeSets.IsEmpty())
	{
		for (const TSubclassOf<UAttributeSet> AttributeSetClass : AbilitySystemInitializationData.AttributeSets)
		{
			GetOrCreateAttributeSet(AttributeSetClass);
		}
	}

	// Set base attribute values if the map isn't empty.
	if (!AbilitySystemInitializationData.AttributeBaseValues.IsEmpty())
	{
		for (const TTuple<FGameplayAttribute, FMKAttributeInitializer>& AttributeBaseValue :
			 AbilitySystemInitializationData.
			 AttributeBaseValues)
		{
			if (HasAttributeSetForAttribute(AttributeBaseValue.Key))
			{
				SetNumericAttributeBase(AttributeBaseValue.Key, AttributeBaseValue.Value.Value.GetValueAtLevel(0.f));
			}
		}
	}

	// Grant Gameplay Abilities if the array isn't empty.
	if (!AbilitySystemInitializationData.GameplayAbilities.IsEmpty())
	{
		for (const TSubclassOf<UGameplayAbility> GameplayAbility : AbilitySystemInitializationData.GameplayAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(GameplayAbility, 0, INDEX_NONE, this);

			GiveAbility(AbilitySpec);
		}
	}

	// Apply Gameplay Effects if the array isn't empty.
	if (!AbilitySystemInitializationData.GameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& GameplayEffect : AbilitySystemInitializationData.GameplayEffects)
		{
			if (!IsValid(GameplayEffect))
			{
				continue;
			}

			FGameplayEffectContextHandle EffectContextHandle = MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);

			if (FGameplayEffectSpecHandle GameplayEffectSpecHandle =
					MakeOutgoingSpec(GameplayEffect, 1, EffectContextHandle); GameplayEffectSpecHandle.IsValid())
			{
				ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), this);
			}
		}
	}

	// ChangeLevel(GetNumericAttribute(ULevelAttributeSet::GetCurrentLevelAttribute()), true);
}

void UMKAbilitySystemComponent::TryActivateAbilityByName(FName AbilityName)
{
	if (auto Handle = AbilityHandles.Find(AbilityName))
	{
		TryActivateAbility(*Handle);
	}
}

const UAttributeSet* UMKAbilitySystemComponent::GetOrCreateAttributeSet(const TSubclassOf<UAttributeSet>& InAttributeSet)
{
	return GetOrCreateAttributeSubobject(InAttributeSet);
}

AMKAbilityCharacter* UMKAbilitySystemComponent::GetAbilityCharacter() const
{
	return Cast<AMKAbilityCharacter>(GetAvatarActor_Direct());
}

UMKAbilitySystemComponent* UMKAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor,
	bool                                                                                               LookForComponent)
{
	return Cast<UMKAbilitySystemComponent>(
										   UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(
												Actor,
												LookForComponent));
}

void UMKAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer&        GameplayTagContainer,
														   TArray<UMKAbilitySystemComponent*>& ActiveAbilities) const
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActive;

	// 태그와 관련된 어빌리티 모두 반환
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActive, false);

	// 관련된 어빌리티에서 현재 활성화 된게 있는지 확인할 것
	for (FGameplayAbilitySpec* Spec : AbilitiesToActive)
	{
		TArray<UGameplayAbility*> ActiveInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : ActiveInstances)
		{
			ActiveAbilities.Add(Cast<UMKAbilitySystemComponent>(ActiveAbility));
		}
	}
}

void UMKAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UMKAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UMKAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	static TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	AbilitiesToActivate.Reset();

	//
	// Process all abilities that activate when the input is held.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const UMKGameplayAbility* SNAbility = CastChecked<UMKGameplayAbility>(AbilitySpec->Ability);

				if (SNAbility->GetActivationType() == EMKAbilityActivationType::WhileInputActive)
				{
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	//
	// Process all abilities that had their input pressed this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		// UE 5.5부터 어빌리티의 Instancing Policy가 Instance되면 Current로 받아야 한다.
		if (FGameplayAbilitySpec* AbilitySpecCDO = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (UGameplayAbility* AbilityInstance = AbilitySpecCDO->GetPrimaryInstance())
			{
				const UMKGameplayAbility* CustomGameplayAbility = Cast<UMKGameplayAbility>(AbilityInstance);
				check(CustomGameplayAbility);

				FGameplayAbilitySpec* InstancedAbilitySpec = CustomGameplayAbility->GetCurrentAbilitySpec();

				InstancedAbilitySpec->InputPressed = true;

				if (InstancedAbilitySpec->IsActive() && !CustomGameplayAbility->bTriggerWhileActive)
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*InstancedAbilitySpec);

					if (InstancedAbilitySpec->IsActive())
					{
						// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
						InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed,
											  InstancedAbilitySpec->Handle,
											  CustomGameplayAbility->GetCurrentActivationInfo().
																	 GetActivationPredictionKey());
					}
				}
				else
				{
					if (CustomGameplayAbility->GetActivationType() == EMKAbilityActivationType::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(InstancedAbilitySpec->Handle);
					}
				}
			}
		}
	}

	//
	// Try to activate all the abilities that are from presses and holds.
	// We do it all at once so that held inputs don't activate the ability
	// and then also send a input event to the ability because of the press.
	//
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	//
	// Process all abilities that had their input released this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpecCDO = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (UGameplayAbility* GameplayAbilityInstanced = AbilitySpecCDO->GetPrimaryInstance())
			{
				FGameplayAbilitySpec* Spec = GameplayAbilityInstanced->GetCurrentAbilitySpec();
				Spec->InputPressed         = false;

				if (Spec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*Spec);
					if (Spec->IsActive())
					{
						// InputRelease 같은 Task 에서 Release 됐을 때 이벤트를 호출한다.
						InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased,
											  Spec->Handle,
											  GameplayAbilityInstanced->GetCurrentActivationInfoRef().
																		GetActivationPredictionKey());
					}
				}
			}
		}
	}

	//
	// Clear the cached ability handles.
	//
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
}

void UMKAbilitySystemComponent::ClearAbilityInput()
{
	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	InputHeldSpecHandles.Reset();
}

void UMKAbilitySystemComponent::ReceiveDamage(UMKAbilitySystemComponent* SourceASC, float UnmitigatedDamage,
											  float                      MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}

void UMKAbilitySystemComponent::AddSpecHandle(FName AbilityName, const FGameplayAbilitySpecHandle& Handle)
{
	AbilityHandles.Emplace(AbilityName, Handle);
}

void UMKAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

}

void UMKAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);
	// if (Spec.IsActive())
	// {
	// 	// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
	// 	InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased,
	// 						  Spec.Handle,
	// 						  FPredictionKey());
	// }
}

void UMKAbilitySystemComponent::OnGameplayEffectAddedCallback(UAbilitySystemComponent* const ASC,
															  const FGameplayEffectSpec&     Spec,
															  FActiveGameplayEffectHandle    Handle)
{
	if (const FActiveGameplayEffect* ActiveEffect = GetActiveGameplayEffect(Handle))
	{
		OnMKGameplayEffectEventDelegate.Broadcast(*ActiveEffect, EASEffectEventType::Added);
	}
}

void UMKAbilitySystemComponent::OnGameplayEffectRemovedCallback(const FActiveGameplayEffect& ActiveGameplayEffect)
{
	OnMKGameplayEffectEventDelegate.Broadcast(ActiveGameplayEffect, EASEffectEventType::Removed);

}
