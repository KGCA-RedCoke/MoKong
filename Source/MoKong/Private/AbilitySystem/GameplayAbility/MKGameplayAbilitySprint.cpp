// CopyRight KGCA - Team RedCoke


#include "AbilitySystem/GameplayAbility/MKGameplayAbilitySprint.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "AttributeSets/AttributeSet_Stamina.h"
#include "Component/LocomotionComponent.h"
#include "GameFramework/Character.h"

void UMKGameplayAbilitySprint::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	CharacterRef        = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	LocomotionComponent = CharacterRef->FindComponentByClass<ULocomotionComponent>();

	GetAbilitySystemComponentFromActorInfo()->
			GetGameplayAttributeValueChangeDelegate(UAttributeSet_Stamina::GetCurrentStaminaAttribute())
			.AddUObject(this, &UMKGameplayAbilitySprint::OnStaminaChange);
}

bool UMKGameplayAbilitySprint::CommitCheck(const FGameplayAbilitySpecHandle     Handle,
										   const FGameplayAbilityActorInfo*     ActorInfo,
										   const FGameplayAbilityActivationInfo ActivationInfo,
										   FGameplayTagContainer*               OptionalRelevantTags)
{
	if (!Super::CommitCheck(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags))
		return false;

	if (!LocomotionComponent)
		return false;

	// Shift키를 눌렀는데 달리는중이 아니라면 그냥 속도를 높이고 어빌리티는 사용하지 않는다.
	if (LocomotionComponent->GetDesiredGaitProfile() < EActionProfile::Mid)
	{
		LocomotionComponent->HandleHighProfileAction(true, 0.f);
		return false;
	}
	const float CurrentStamina = GetAbilitySystemComponentFromActorInfo()->
			GetNumericAttribute(UAttributeSet_Stamina::GetCurrentStaminaAttribute());

	return CurrentStamina > 0.f;
}

void UMKGameplayAbilitySprint::ActivateAbility(const FGameplayAbilitySpecHandle     Handle,
											   const FGameplayAbilityActorInfo*     ActorInfo,
											   const FGameplayAbilityActivationInfo ActivationInfo,
											   const FGameplayEventData*            TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	LocomotionComponent->HandleHighProfileAction(true, 0.f);

	UAbilityTask_WaitInputRelease* InputTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
	InputTask->OnRelease.AddUniqueDynamic(this, &UMKGameplayAbilitySprint::OnReleaseInput);
	InputTask->ReadyForActivation();

	if (LocomotionComponent->GetDesiredGaitProfile() == EActionProfile::High)
	{
		SprintCostEffectHandle       = MakeOutgoingGameplayEffectSpec(SprintCostEffectClass);
		SprintCostActiveEffectHandle = ApplyGameplayEffectSpecToOwner(Handle,
																	  ActorInfo,
																	  ActivationInfo,
																	  SprintCostEffectHandle);
	}

}

void UMKGameplayAbilitySprint::EndAbility(const FGameplayAbilitySpecHandle     Handle,
										  const FGameplayAbilityActorInfo*     ActorInfo,
										  const FGameplayAbilityActivationInfo ActivationInfo,
										  bool                                 bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	LocomotionComponent->HandleHighProfileAction(false, 0.f);
	GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(SprintCostActiveEffectHandle, -1);
}

void UMKGameplayAbilitySprint::OnStaminaChange(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (OnAttributeChangeData.NewValue <= 0.f)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UMKGameplayAbilitySprint::OnReleaseInput(float TimeHeld)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
