// CopyRight KGCA - Team RedCoke


#include "CombatSystem/Ability/MKPrimaryAttackAbility.h"

#include "AbilitySystemComponent.h"
#include "MKGameplayTags.h"
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
	CombatSystem->UpdateComboData(CurrentMontage, SectionName, false);

	FGameplayTagContainer EventTags;
	EventTags.AddTag(FGameplayTag::RequestGameplayTag("Event.Montage.NextSection"));
	EventTags.AddTag(FGameplayTag::RequestGameplayTag("Event.Montage.End"));

	if (SectionName == "1" && GetAbilitySystemComponentFromActorInfo()->
		HasMatchingGameplayTag(MoKong::Action::TAG_Action_Sprint))
	{
		SectionName = "1_Sprint";
	}

	MontageTask = UAbilityTask_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
		 this,
		 FName("Default"),
		 CurrentMontage.Get(),
		 EventTags,
		 1.f,
		 SectionName,
		 true);

	MontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnCompleted);
	MontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnCancelled);
	MontageTask->EventReceived.AddDynamic(this, &ThisClass::EventReceived);

	MontageTask->ReadyForActivation();

	// FRotator TargetRotation;
	// TargetRotation.Pitch = AvatarCharacter->GetActorRotation().Pitch;
	// TargetRotation.Roll  = AvatarCharacter->GetActorRotation().Roll;
	//
	// // 타게팅 된 액터가 있으면 타겟을 향해 회전
	// if (AActor* Target = CombatSystem->GetTargetActor_Implementation())
	// {
	// 	TargetRotation.Yaw = UKismetMathLibrary::FindLookAtRotation(
	// 																AvatarCharacter->GetActorLocation(),
	// 																Target->GetActorLocation()).Yaw;
	//
	// 	AvatarCharacter->UpdateMotionWarpingTargetLocationAndRotation(
	// 																  "FindTarget",
	// 																  AvatarCharacter->GetActorLocation(),
	// 																  TargetRotation);
	// }
	// // 아니면 플레이어라면? 플레이어 컨틀롤러 방향으로 회전
	// else if (GetAbilitySystemComponentFromActorInfo()->
	// 	HasMatchingGameplayTag(MoKong::CharacterTags::TAG_Character_Type_PC))
	// {
	// 	TargetRotation.Yaw = AvatarCharacter->GetControlRotation().Yaw;
	//
	// 	AvatarCharacter->UpdateMotionWarpingTargetLocationAndRotation(
	// 																  "FindTarget",
	// 																  AvatarCharacter->GetActorLocation(),
	// 																  TargetRotation);
	// }


	bShouldEndAbility = true;
}

void UMKPrimaryAttackAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (EventTag.MatchesTag(FGameplayTag::RequestGameplayTag("Event.Montage.End")))
	{
		AvatarCharacter->PlayAnimMontage(EndMontage.Get(), 1.f, SectionName);

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

void UMKPrimaryAttackAbility::EndAbility(const FGameplayAbilitySpecHandle     Handle,
										 const FGameplayAbilityActorInfo*     ActorInfo,
										 const FGameplayAbilityActivationInfo ActivationInfo,
										 bool                                 bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CombatSystem->ResetComboData();
	MontageTask->EndTask();
}
