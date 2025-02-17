// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MKAttributeSet.h"
#include "AttributeSet_Mokong.generated.h"

/**
 * 플레이어 기본 Attribute Set
 *
 * Contents:
 *
 *	-------------------- Attributes --------------------
 * Level - 레벨
 * CurrentExp - 현재 경험치
 * TargetExp - 목표 경험치
 * Attack - 공격력
 * Defense - 방어력
 * CriticalHitChance - 치명타 확률 
 * CriticalHitDamage - 치명타 데미지
 * DamageBonus - 추가 피해
 * DamageReduction - 피해 감소
 * TargetFocus - 강공격 게이지
 *
 */
UCLASS()
class ABILITYSYSTEM_API UAttributeSet_Mokong : public UMKAttributeSet
{
	GENERATED_BODY()

public:
	UAttributeSet_Mokong();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue,
										 float                     NewValue) const override;

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos), ReplicatedUsing = OnRep_Level)
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Mokong, Level)

	// 경험치
	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos), ReplicatedUsing = OnRep_CurrentExp)
	FGameplayAttributeData CurrentExp;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Mokong, CurrentExp)

	// 목표 경험치
	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos), ReplicatedUsing = OnRep_TargetExp)
	FGameplayAttributeData TargetExp;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Mokong, TargetExp)

	/** 공격력 */
	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos), ReplicatedUsing = OnRep_Attack)
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Mokong, Attack)

	/** 방어력 */
	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos), ReplicatedUsing = OnRep_Defense)
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Mokong, Defense)

	/** 치명타 확률 */
	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos), ReplicatedUsing = OnRep_CriticalHitChance)
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Mokong, CriticalHitChance)

	/** 치명타 데미지 */
	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos), ReplicatedUsing = OnRep_CriticalHitDamage)
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Mokong, CriticalHitDamage)

	/** 추가 피해 */
	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos), ReplicatedUsing = OnRep_DamageBonus)
	FGameplayAttributeData DamageBonus;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Mokong, DamageBonus)

	/** 피해 감소 */
	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos), ReplicatedUsing = OnRep_DamageReduction)
	FGameplayAttributeData DamageReduction;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Mokong, DamageReduction)

	/** 강공격 게이지 */
	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos), ReplicatedUsing = OnRep_TargetFocus)
	FGameplayAttributeData TargetFocus;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Mokong, TargetFocus)

	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos), ReplicatedUsing = OnRep_CurrentFocus)
	FGameplayAttributeData CurrentFocus;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Mokong, CurrentFocus)

protected:
	UFUNCTION()
	virtual void OnRep_Level(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_CurrentExp(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_TargetExp(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Attack(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_Defense(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_DamageBonus(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_DamageReduction(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_TargetFocus(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_CurrentFocus(const FGameplayAttributeData& OldValue);
};
