// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "MKGameplayEffectApplicationRequirement.generated.h"

struct FGameplayEffectSpec;
class UGameplayEffect;
class UAbilitySystemComponent;

/**
 * 
 */
UCLASS(Blueprintable)
class ABILITYSYSTEM_API UMKGameplayEffectApplicationRequirement : public UGameplayEffectComponent
{

	GENERATED_BODY()

public:
	/** Constructor to set EditorFriendlyName */
	UMKGameplayEffectApplicationRequirement();

	virtual bool CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer,
										const FGameplayEffectSpec&             GESpec) const override;

	/** Return whether the gameplay effect should be applied or not */
	UFUNCTION(BlueprintNativeEvent, Category="Calculation", DisplayName="CanApplyGameplayEffect")
	bool K0_CanApplyGameplayEffect(const UGameplayEffect* const         GameplayEffect, const FGameplayEffectSpec& Spec,
								   const UAbilitySystemComponent* const ASC) const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};
