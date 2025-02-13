// CopyRight KGCA - Team RedCoke


#include "Ability/MKGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Ability/MKAbilitySystemComponent.h"
#include "Data/MKAbilitySystemData.h"
#include "Data/MKTargetType.h"
#include "GameFramework/Character.h"

UMKGameplayAbility::UMKGameplayAbility()
	: bStartActiveImmediately(false),
	  bTriggerWhileActive(false)
{
	// 액터마다 어빌리티 인스턴스는 하나 가짐 (상태가 저장됨)
	InstancingPolicy   = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly; // 싱글 게임은 서버에서만 실행
}

void UMKGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (bStartActiveImmediately)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}

	AvatarCharacter = Cast<AMKAbilityCharacter>(ActorInfo->AvatarActor);
}

FMKGameplayEffectContainerSpec UMKGameplayAbility::MakeEffectContainerSpecFromContainer(
	const FMKGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FMKGameplayEffectContainerSpec ReturnSpec;
	AActor*                        OwningActor = GetOwningActorFromActorInfo();
	ACharacter*                    OwningCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	UMKAbilitySystemComponent*     OwningASC = UMKAbilitySystemComponent::GetAbilitySystemComponentFromActor(OwningActor);

	if (OwningASC)
	{
		if (Container.TargetType.Get())
		{
			TArray<FHitResult>   HitResults;
			TArray<AActor*>      TargetActors;
			const UMKTargetType* TargetTypeCD0 = Container.TargetType.GetDefaultObject();
			AActor*              AvatarActor   = GetAvatarActorFromActorInfo();
			TargetTypeCD0->GetTargets(OwningCharacter, AvatarActor, EventData, HitResults, TargetActors);
			ReturnSpec.AddTargets(HitResults, TargetActors);
		}

		if (OverrideGameplayLevel == INDEX_NONE)
		{
			OverrideGameplayLevel = OverrideGameplayLevel = this->GetAbilityLevel();
		}

		for (const TSubclassOf<UGameplayEffect>& EffectClass : Container.TargetGameplayEffectClasses)
		{
			ReturnSpec.TargetGameplayEffectSpecs.
					   Add(MakeOutgoingGameplayEffectSpec(EffectClass, OverrideGameplayLevel));
		}
	}

	return ReturnSpec;
}

FMKGameplayEffectContainerSpec UMKGameplayAbility::MakeEffectContainerSpec(FGameplayTag              ContainerTag,
																		   const FGameplayEventData& EventData,
																		   int32                     OverrideGameplayLevel)
{
	if (const FMKGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag))
	{
		return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	}
	return FMKGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> UMKGameplayAbility::ApplyEffectContainerSpec(
	const FMKGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;

	// Iterate list of effect specs and apply them to their target data
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
	}
	return AllEffects;
}

TArray<FActiveGameplayEffectHandle> UMKGameplayAbility::ApplyEffectContainer(FGameplayTag ContainerTag,
																			 const FGameplayEventData& EventData,
																			 int32 OverrideGameplayLevel)
{
	const FMKGameplayEffectContainerSpec Spec =
			MakeEffectContainerSpec(ContainerTag, EventData, OverrideGameplayLevel);
	return ApplyEffectContainerSpec(Spec);
}

void UMKGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle     Handle,
										 const FGameplayAbilityActorInfo*     ActorInfo,
										 const FGameplayAbilityActivationInfo ActivationInfo,
										 const FGameplayEventData*            TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (CurrentActorInfo)
	{
		const UAnimInstance* AnimInstance = CurrentActorInfo->SkeletalMeshComponent->GetAnimInstance();
		if (AnimInstance && AnimInstance->GetCurveValue("DisableInput") > 0.f)
		{
			bInputLocked = true;
			CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		}
		else
		{
			bInputLocked = false;
		}
	}
}
