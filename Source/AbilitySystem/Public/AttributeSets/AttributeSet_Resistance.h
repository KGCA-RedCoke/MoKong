// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MKAttributeSet.h"
#include "AttributeSet_Resistance.generated.h"

/**
 *  Contains Attributes related to Resistance. Any Character that uses Resistance will need this Attribute Set.
 *
 *	Contents:
 *	Resistance - Primarily used as reduction / increase to damage or status
 */
UCLASS()
class ABILITYSYSTEM_API UAttributeSet_Resistance : public UMKAttributeSet
{
	GENERATED_BODY()

public:
	UAttributeSet_Resistance();

	inline static float RESISTANCE_BASE = 100.f;
	inline static float RESISTANCE_MAX  = 200.f;
	inline static float RESISTANCE_MIN  = -75.f;

	// Attribute Set Overrides.
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue,
										 float                     NewValue) const override;

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Holds the current value for Resistance.
	UPROPERTY(BlueprintReadOnly, Category = "Resistance Attribute Set", ReplicatedUsing = OnRep_Resistance)
	FGameplayAttributeData Resistance;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Resistance, Resistance)

	UFUNCTION(BlueprintPure, Category = "Resistance Attribute Set")
	static float GetBaseResistance() { return RESISTANCE_BASE; }

	UFUNCTION(BlueprintPure, Category = "Resistance Attribute Set")
	static FVector2D GetMinMaxResistance() { return FVector2D(RESISTANCE_MIN, RESISTANCE_MAX); }

protected:
	UFUNCTION()
	virtual void OnRep_Resistance(const FGameplayAttributeData& OldValue);
};
