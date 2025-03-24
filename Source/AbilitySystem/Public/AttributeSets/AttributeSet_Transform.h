// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MKAttributeSet.h"
#include "AttributeSet_Transform.generated.h"

/**
* 플레이어 변신 Attribute Set
*
* Contents:
*
*	-------------------- Attributes --------------------
* TransformCurrentHealth - 변신 체력 (현재 체력)
* TransformMaximumHealth - 변신 체력 (최대 체력)
* TransformCurrentStamina - 변신 스태미너 (현재 스태미너)
* TransformMaximumStamina - 변신 스태미너 (최대 스태미너)
* 
*/
UCLASS()
class ABILITYSYSTEM_API UAttributeSet_Transform : public UMKAttributeSet
{
	GENERATED_BODY()

public:
	UAttributeSet_Transform();
	
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue,
										 float                     NewValue) const override;

	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos), ReplicatedUsing = OnRep_TransformCurrentHealth)
	FGameplayAttributeData TransformCurrentHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Transform, TransformCurrentHealth)

	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos), ReplicatedUsing = OnRep_TransformMaximumHealth)
	FGameplayAttributeData TransformMaximumHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Transform, TransformMaximumHealth)

	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos), ReplicatedUsing = OnRep_TransformCurrentStamina)
	FGameplayAttributeData TransformCurrentStamina;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Transform, TransformCurrentStamina)

	UPROPERTY(BlueprintReadOnly, meta = (HideFromLevelInfos), ReplicatedUsing = OnRep_TransformMaximumStamina)
	FGameplayAttributeData TransformMaximumStamina;
	ATTRIBUTE_ACCESSORS(UAttributeSet_Transform, TransformMaximumStamina)

protected:
	UFUNCTION()
	virtual void OnRep_TransformCurrentHealth(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_TransformMaximumHealth(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_TransformCurrentStamina(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_TransformMaximumStamina(const FGameplayAttributeData& OldValue);

};
