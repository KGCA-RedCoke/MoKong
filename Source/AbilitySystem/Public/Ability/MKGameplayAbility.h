// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Data/MKAbilitySystemData.h"
#include "MKGameplayAbility.generated.h"

class AMKAbilityCharacter;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API UMKGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMKGameplayAbility();

	FORCEINLINE EMKAbilityActivationType GetActivationType() const { return ActivationType; }
	FORCEINLINE FName                    GetAbilityID() const { return AbilityID; }

	// Beginplay와 비슷한 열할 (패시브 스킬이나 시작하자마자 어빌리티를 부여하고 싶을 때 여기서 처리)
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UFUNCTION(BlueprintCallable, Category = Ability, meta=(AutoCreateRefTerm = "EventData"))
	virtual FMKGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(
		const FMKGameplayEffectContainer& Container,
		const FGameplayEventData&         EventData, int32 OverrideGameplayLevel = -1);

	/** Search for and make a gameplay effect container spec to be applied later, from the EffectContainerMap */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual FMKGameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag              ContainerTag,
																   const FGameplayEventData& EventData,
																   int32                     OverrideGameplayLevel = -1);

	/** Applies a gameplay effect container spec that was previously created */
	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(
		const FMKGameplayEffectContainerSpec& ContainerSpec);

	/** Applies a gameplay effect container, by creating and then applying the spec */
	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
	virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(FGameplayTag              ContainerTag,
																	 const FGameplayEventData& EventData,
																	 int32                     OverrideGameplayLevel = -1);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle     Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData*            TriggerEventData) override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	FName AbilityID;	// 별칭 (Handle만 가지고 있으면 어빌리티를 찾을 수 있지만, 별칭을 가지고 있으면 더 쉽게 찾을 수 있다.)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool bStartActiveImmediately;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool bTriggerWhileActive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayEffects)
	TMap<FGameplayTag, FMKGameplayEffectContainer> EffectContainerMap;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InputSystem|Ability Activation")
	EMKAbilityActivationType ActivationType;

	UPROPERTY()
	bool bInputLocked;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	TObjectPtr<AMKAbilityCharacter> AvatarCharacter;
};
