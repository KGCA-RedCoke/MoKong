// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MKAttributeSet.h"
#include "AttributeSet_Mana.generated.h"

/**
 * 주문과 관련된 Attribute Set
 *
 * Contents:
 *
 *	-------------------- Attributes --------------------
 * Current Health - 현재 캐릭터의 체력 (Current)
 * Maximum Health - 최대 체력 (Base)
 * Health Regeneration - 체력 재생 비율 (Base)
 *
 * 아래의 메타 어트리뷰트는 그냥 어트리뷰트와는 성질이 다르다.
 * -------------------- Meta Attributes --------------------
 * Damage - Health에 영향을 주는 메타 어트리뷰트 (-)
 * Healing - Health에 영향을 주는 메타 어트리뷰트 (+)
 */
UCLASS()
class ABILITYSYSTEM_API UAttributeSet_Mana : public UMKAttributeSet
{
	GENERATED_BODY()

public:
	UAttributeSet_Mana();

	// Attribute Set Overrides.
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, Category = "Mana Attribute Set", ReplicatedUsing = OnRep_CurrentMana)
	FGameplayAttributeData CurrentMana;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Mana, CurrentMana)

	UPROPERTY(BlueprintReadOnly, Category = "Mana Attribute Set", ReplicatedUsing = OnRep_MaximumMana)
	FGameplayAttributeData MaximumMana;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Mana, MaximumMana)

	UPROPERTY(BlueprintReadOnly, Category = "Mana Attribute Set", ReplicatedUsing = OnRep_ManaRegeneration)
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Mana, ManaRegeneration)

protected:
	UFUNCTION()
	virtual void OnRep_CurrentMana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaximumMana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue);
};
