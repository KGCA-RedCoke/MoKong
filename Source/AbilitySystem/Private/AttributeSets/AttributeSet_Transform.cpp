// CopyRight KGCA - Team RedCoke


#include "AttributeSets/AttributeSet_Transform.h"

UAttributeSet_Transform::UAttributeSet_Transform() {}


void UAttributeSet_Transform::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);


}

void UAttributeSet_Transform::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UAttributeSet_Transform::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue,
													  float                     NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UAttributeSet_Transform::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributes(Attribute, NewValue);
}

void UAttributeSet_Transform::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UAttributeSet_Transform::OnRep_TransformCurrentHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Transform, TransformCurrentHealth, OldValue);
}

void UAttributeSet_Transform::OnRep_TransformMaximumHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Transform, TransformMaximumHealth, OldValue);
}

void UAttributeSet_Transform::OnRep_TransformCurrentStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Transform, TransformCurrentStamina, OldValue);
}

void UAttributeSet_Transform::OnRep_TransformMaximumStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Transform, TransformMaximumStamina, OldValue);
}
