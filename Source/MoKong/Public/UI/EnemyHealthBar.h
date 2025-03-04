// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Ability/AbilityBindWidgetBase.h"
#include "EnemyHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class MOKONG_API UEnemyHealthBar : public UAbilityBindWidgetBase
{
	GENERATED_BODY()

public:
	virtual bool InitializeAbilitySystemWidget(UAbilitySystemComponent* OwnerAbilitySystemComponent) override;
	
	// Event called when the Maximum Health attribute value changes.
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability System")
	void On_MaximumHealthChanged(const float NewValue, const float OldValue, const float NewPercentage);

	// Event called when the Current Health attribute value changes.
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability System")
	void On_CurrentHealthChanged(const float NewValue, const float OldValue, const float NewPercentage);

	// Event called when the Armor attribute value changes.
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability System")
	void On_ResistanceChanged(const float NewBase, const float NewMagnitude);

protected:
	FDelegateHandle MaximumHealthChangeDelegate;
	FDelegateHandle CurrentHealthChangeDelegate;
	FDelegateHandle ResistanceChangeDelegate;

	void MaximumHealthChanged(const FOnAttributeChangeData& Data);
	void CurrentHealthChanged(const FOnAttributeChangeData& Data);
	void ResistanceChanged(const FOnAttributeChangeData& Data);

};
