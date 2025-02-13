// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Ability/MKGameplayAbility.h"
#include "MKGameplayAbilityJump.generated.h"

class ULocomotionComponent;
/**
 * 
 */
UCLASS()
class MOKONG_API UMKGameplayAbilityJump : public UMKGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CommitCheck(const FGameplayAbilitySpecHandle     Handle, const FGameplayAbilityActorInfo* ActorInfo,
							 const FGameplayAbilityActivationInfo ActivationInfo,
							 FGameplayTagContainer*               OptionalRelevantTags = nullptr) override;

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle     Handle, const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
							bool                                 bWasCancelled) override;

	UFUNCTION()
	void OnCharacterLanded(const FHitResult& Hit);
	UFUNCTION()
	void OnReleaseJump(float TimeHeld);

private:
	TObjectPtr<ACharacter>           CharacterRef;
	TObjectPtr<ULocomotionComponent> LocomotionComponent;
};
