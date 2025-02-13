// CopyRight KGCA - Team RedCoke


#include "AbilitySystem/GameplayAbility/MKGameplayAbilityJump.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Component/LocomotionComponent.h"
#include "GameFramework/Character.h"

void UMKGameplayAbilityJump::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if ((CharacterRef = Cast<ACharacter>(GetAvatarActorFromActorInfo())))
	{
		CharacterRef->LandedDelegate.AddUniqueDynamic(this, &UMKGameplayAbilityJump::OnCharacterLanded);
	}

	LocomotionComponent = CharacterRef->FindComponentByClass<ULocomotionComponent>();
}

bool UMKGameplayAbilityJump::CommitCheck(const FGameplayAbilitySpecHandle     Handle,
										 const FGameplayAbilityActorInfo*     ActorInfo,
										 const FGameplayAbilityActivationInfo ActivationInfo,
										 FGameplayTagContainer*               OptionalRelevantTags)
{
	if (!Super::CommitCheck(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags))
		return false;

	return LocomotionComponent.Get() && CharacterRef->CanJump();
}

void UMKGameplayAbilityJump::ActivateAbility(const FGameplayAbilitySpecHandle     Handle,
											 const FGameplayAbilityActorInfo*     ActorInfo,
											 const FGameplayAbilityActivationInfo ActivationInfo,
											 const FGameplayEventData*            TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}

	LocomotionComponent->HandleJumpAction(true, 0.f);

	UAbilityTask_WaitInputRelease* Task = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
	Task->OnRelease.AddDynamic(this, &UMKGameplayAbilityJump::OnReleaseJump);
	Task->ReadyForActivation();
}

void UMKGameplayAbilityJump::EndAbility(const FGameplayAbilitySpecHandle     Handle,
										const FGameplayAbilityActorInfo*     ActorInfo,
										const FGameplayAbilityActivationInfo ActivationInfo,
										bool                                 bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	LocomotionComponent->HandleJumpAction(false, 0.f);
}

void UMKGameplayAbilityJump::OnCharacterLanded(const FHitResult& Hit)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);

}

void UMKGameplayAbilityJump::OnReleaseJump(float TimeHeld)
{
	LocomotionComponent->HandleJumpAction(false, 0.f);
}
