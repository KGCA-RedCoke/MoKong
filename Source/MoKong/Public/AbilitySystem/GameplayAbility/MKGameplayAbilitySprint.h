// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Ability/MKGameplayAbility.h"
#include "MKGameplayAbilitySprint.generated.h"

class ULocomotionComponent;
/**
 * 
 */
UCLASS()
class MOKONG_API UMKGameplayAbilitySprint : public UMKGameplayAbility
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

	void OnStaminaChange(const FOnAttributeChangeData& OnAttributeChangeData);

	UFUNCTION()
	void OnReleaseInput(float TimeHeld);

private:
	TObjectPtr<ACharacter>           CharacterRef;
	TObjectPtr<ULocomotionComponent> LocomotionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Sprint|Effect")
	TSubclassOf<UGameplayEffect> SprintCostEffectClass;

	FGameplayEffectSpecHandle   SprintCostEffectHandle;
	FActiveGameplayEffectHandle SprintCostActiveEffectHandle;
};
