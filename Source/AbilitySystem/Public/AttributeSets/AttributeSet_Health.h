// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MKAttributeSet.h"
#include "Data/MKAbilitySystemData.h"
#include "AttributeSet_Health.generated.h"

/**
 * 체력과 관련된 Attribute Set
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
class ABILITYSYSTEM_API UAttributeSet_Health : public UMKAttributeSet
{
	GENERATED_BODY()

public:
	UAttributeSet_Health();

	// Attribute Set Overrides.
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue,
										 float                     NewValue) const override;

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Used to create a local copy of Damage which is then subtracted from Current Health.
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", meta = (HideFromLevelInfos))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Health, Damage)

	// Used to create a local copy of Healing which is then added to Current Health.
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", meta = (HideFromLevelInfos))
	FGameplayAttributeData Healing;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Health, Healing)

	// Used to create a local copy of Bleed which is then added to Bleeding.
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", meta = (HideFromLevelInfos))
	FGameplayAttributeData ReceivedBleed;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Health, ReceivedBleed)

	// Used to create a local copy of Direct Damage which is then added to Current Health.
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", meta = (HideFromLevelInfos))
	FGameplayAttributeData ReceivedDirectDamage;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Health, ReceivedDirectDamage)

	// 체력의 현재 값
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_CurrentHealth)
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Health, CurrentHealth)

	// 체력의 최대 값
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_MaximumHealth)
	FGameplayAttributeData MaximumHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Health, MaximumHealth)

	// 체력 재생 비율
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_HealthRegeneration)
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Health, HealthRegeneration)

	// 현재 출혈량
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_Bleeding)
	FGameplayAttributeData Bleeding;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Health, Bleeding)

	// 현재 출혈 회복량
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_BleedHealing)
	FGameplayAttributeData BleedHealing;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Health, BleedHealing)

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FGameplayAttribute, FMKAttributeMaxValue> AttributeMaxValue;

	UFUNCTION()
	virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaximumHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Bleeding(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_BleedHealing(const FGameplayAttributeData& OldValue);
};
