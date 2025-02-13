// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AttributeSet.h"
#include "MKAbilitySystemBlueprintLibrary.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UAttributeSet;
enum class EAttributeSearchType : uint8;

UCLASS()
class ABILITYSYSTEM_API UMKAbilitySystemBlueprintLibrary : public UAbilitySystemBlueprintLibrary
{
	GENERATED_BODY()

public:
	// Tries to find the Actor's Ability System Component using the IAbilitySystemInterface.
	// If the Ability System Component is found; attempts to find the value of the Attribute supplied.
	UFUNCTION(BlueprintCallable, Category = "Ability System")
	static float GetAttributeValueFromActor(const AActor* const        Actor, const FGameplayAttribute Attribute,
											const EAttributeSearchType SearchType);

	// Attempts to find the value of the Attribute supplied.
	UFUNCTION(BlueprintCallable, Category = "Ability System")
	static float GetAttributeValueFromAbilitySystem(const UAbilitySystemComponent* const AbilitySystemComponent,
													const FGameplayAttribute             Attribute,
													const EAttributeSearchType           SearchType);

	// Return damage multiplier
	UFUNCTION(BlueprintCallable, Category = "Ability System")
	static float CalculateEffectiveResistance(const float CurrentArmor);

private:
	static void GetAttributeValue(const UAbilitySystemComponent* const AbilitySystemComponent,
								  const FGameplayAttribute&            Attribute, const EAttributeSearchType SearchType,
								  OUT float&                           ReturnValue);
};
