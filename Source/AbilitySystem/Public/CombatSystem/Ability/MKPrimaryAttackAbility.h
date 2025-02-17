// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Ability/MKGameplayAbility.h"
#include "MKPrimaryAttackAbility.generated.h"

class UAbilityTask_PlayMontageAndWaitForEvent;
class UCombatSystem;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API UMKPrimaryAttackAbility : public UMKGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CommitCheck(const FGameplayAbilitySpecHandle     Handle, const FGameplayAbilityActorInfo* ActorInfo,
							 const FGameplayAbilityActivationInfo ActivationInfo,
							 FGameplayTagContainer*               OptionalRelevantTags = nullptr) override;
	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnInputPressed(float TimeWaited);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle     Handle, const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
							bool                                 bWasCancelled) override;

	void Do_AttackTask();

private:
	UPROPERTY(Meta = (AllowPrivateAccess = true))
	TObjectPtr<UCombatSystem> CombatSystem;

	bool bShouldEndAbility = false;

	FName SectionName;

	TObjectPtr<UAbilityTask_PlayMontageAndWaitForEvent> MontageTask;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> EndMontage;
};
