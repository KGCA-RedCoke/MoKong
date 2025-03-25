// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Ability/AbilityBindWidgetBase.h"
#include "PlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOKONG_API UPlayerHUDWidget : public UAbilityBindWidgetBase
{
	GENERATED_BODY()

public:
	virtual bool InitializeAbilitySystemWidget(UAbilitySystemComponent* OwnerAbilitySystemComponent) override;

	// Event called when the Maximum Health attribute value changes.
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability System")
	void On_MaximumHealthChanged(const float NewValue, const float OldValue, const float NewPercentage);
	// Event called when the  Maximum Stamina attribute value changes.
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability System")
	void On_MaximumStaminaChanged(const float NewValue, const float OldValue, const float NewPercentage);
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability System")
	void On_MaximumManaChanged(const float NewValue, const float OldValue, const float NewPercentage);
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability System")
	void On_MaximumFocusChanged(const int32 NewValue, const int32 OldValue);

	// Event called when the Current Health attribute value changes.
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability System")
	void On_CurrentHealthChanged(const float NewValue, const float OldValue, const float NewPercentage);
	// Event called when the Current Stamina attribute value changes.
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability System")
	void On_CurrentStaminaChanged(const float NewValue, const float OldValue, const float NewPercentage);
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability System")
	void On_CurrentManaChanged(const float NewValue, const float OldValue, const float NewPercentage);
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability System")
	void On_CurrentFocusChanged(const float NewValue, const float OldValue, const float NewPercentage);
	UFUNCTION(BlueprintImplementableEvent, Category= "Ability System")
	void On_CurrentEXPChanged(const float NewValue, const float OldValue, const float NewPercentage);
	UFUNCTION(BlueprintImplementableEvent, Category= "Ability System")
	void On_CurrentLevelChanged(const float NewValue, const float OldValue);

	// Event called when the Health Regeneration attribute value changes.
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability System")
	void On_HealthRegenerationChanged(const float NewValue, const float OldValue);
	// Event called when the Stamina Regeneration attribute value changes.
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability System")
	void On_StaminaRegenerationChanged(const float NewValue, const float OldValue);

	// Event called when the Armor attribute value changes.
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability System")
	void On_ResistanceChanged(const float NewBase, const float NewMagnitude);

	// Event called when the Bleeding attribute value changes.
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability System")
	void On_BleedingChanged(const float NewValue, const float NewHealPerSecond, const float NewRemainingDuration);

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability System")
	void On_FocusFull(const int32 NewCharge);

protected:
	FDelegateHandle MaximumHealthChangeDelegate;
	FDelegateHandle MaximumStaminaChangeDelegate;
	FDelegateHandle MaximumManaChangeDelegate;
	FDelegateHandle MaximumFocusChangeDelegate;

	FDelegateHandle CurrentHealthChangeDelegate;
	FDelegateHandle CurrentStaminaChangeDelegate;
	FDelegateHandle CurrentManaChangeDelegate;
	FDelegateHandle CurrentFocusChangeDelegate;
	FDelegateHandle CurrentEXPChangeDelegate;
	FDelegateHandle CurrentLevelChangeDelegate;

	FDelegateHandle HealthRegenerationChangeDelegate;
	FDelegateHandle StaminaRegenerationChangeDelegate;

	FDelegateHandle ResistanceChangeDelegate;

	FDelegateHandle BleedingChangeDelegate;
	FDelegateHandle BleedHealChangeDelegate;

	void MaximumHealthChanged(const FOnAttributeChangeData& Data);
	void MaximumStaminaChanged(const FOnAttributeChangeData& Data);
	void MaximumManaChanged(const FOnAttributeChangeData& Data);
	void MaximumFocusChanged(const FOnAttributeChangeData& Data);

	void CurrentHealthChanged(const FOnAttributeChangeData& Data);
	void CurrentStaminaChanged(const FOnAttributeChangeData& Data);
	void CurrentManaChanged(const FOnAttributeChangeData& Data);
	void CurrentFocusChanged(const FOnAttributeChangeData& Data);
	void CurrentEXPChanged(const FOnAttributeChangeData& Data);
	void CurrentLevelChanged(const FOnAttributeChangeData& Data);

	void HealthRegenerationChanged(const FOnAttributeChangeData& Data);
	void StaminaRegenerationChanged(const FOnAttributeChangeData& Data);

	void ResistanceChanged(const FOnAttributeChangeData& Data);

	void BleedingChanged(const FOnAttributeChangeData& Data);
};
