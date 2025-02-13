// CopyRight KGCA - Team RedCoke


#include "CombatSystem/Ability/MKPrimaryAttackAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Actor/MKAbilityCharacter.h"
#include "CombatSystem/Components/CombatSystemComp.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tasks/AbilityTask_PlayMontageAndWaitForEvent.h"

void UMKPrimaryAttackAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
}

void UMKPrimaryAttackAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (AvatarCharacter)
	{
		CombatSystem = AvatarCharacter->GetCombatComponent_Implementation();
	}
}

bool UMKPrimaryAttackAbility::CommitCheck(const FGameplayAbilitySpecHandle     Handle,
										  const FGameplayAbilityActorInfo*     ActorInfo,
										  const FGameplayAbilityActivationInfo ActivationInfo,
										  FGameplayTagContainer*               OptionalRelevantTags)
{
	if (!Super::CommitCheck(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags))
	{
		return false;
	}

	return CombatSystem->CheckCommit();
}

void UMKPrimaryAttackAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UMKPrimaryAttackAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (!bShouldEndAbility)
	{
		bShouldEndAbility = true;
		return;
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UMKPrimaryAttackAbility::Do_AttackTask()
{
	FName SectionName;
	CombatSystem->UpdateComboData(CurrentMontage, SectionName, false);

	MontageTask = UAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
		 this,
		 FName("Default"),
		 CurrentMontage.Get(),
		 FGameplayTagContainer(),
		 1.f,
		 SectionName,
		 true);

	MontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnCompleted);
	MontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnCancelled);
	MontageTask->EventReceived.AddDynamic(this, &ThisClass::EventReceived);

	MontageTask->ReadyForActivation();

	if (AActor* Target = CombatSystem->GetTargetActor_Implementation())
	{
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
																		 AvatarCharacter->GetActorLocation(),
																		 Target->GetActorLocation());

		TargetRotation.Pitch = AvatarCharacter->GetActorRotation().Pitch;
		TargetRotation.Roll  = AvatarCharacter->GetActorRotation().Roll;

		AvatarCharacter->UpdateMotionWarpingTargetLocationAndRotation(
																	  "FindTarget",
																	  AvatarCharacter->GetActorLocation(),
																	  TargetRotation);
	}
	bShouldEndAbility = true;
}

void UMKPrimaryAttackAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag.MatchesTag(FGameplayTag::RequestGameplayTag("Event.Montage.End")))
	{
		OnCompleted(EventTag, EventData);
		return;
	}

	if (EventTag.MatchesTag(FGameplayTag::RequestGameplayTag("Event.Montage.NextSection")))
	{
		Do_AttackTask();

		BP_ApplyGameplayEffectToOwner(CostGameplayEffectClass);

		bShouldEndAbility = false;
	}

}

void UMKPrimaryAttackAbility::OnInputPressed(float TimeWaited)
{
	UE_LOG(LogTemp, Warning, TEXT("MKPrimaryAttackAbility::OnInputPressed"));

	if (CombatSystem->CheckCommit())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UMKPrimaryAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle     Handle,
											  const FGameplayAbilityActorInfo*     ActorInfo,
											  const FGameplayAbilityActivationInfo ActivationInfo,
											  const FGameplayEventData*            TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo) || bInputLocked)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

		return;
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	Do_AttackTask();
}

void UMKPrimaryAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CombatSystem->ResetComboData();
	MontageTask->EndTask();
}
