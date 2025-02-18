// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Ability/MKAbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"


class AMokongEnemy;
enum class EMKEffectEventType : uint8;
class UMKAbilitySystemComponent;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FMKEffectEventInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInhibited = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayEffectSpec Spec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<const UGameplayEffect> Def;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActiveGameplayEffect ActiveEffect;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossEncounter, AMokongEnemy*, BossInfo);

/**
 * 
 */
UCLASS()
class MOKONG_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Should this widget bind to Health Attribute Set events.
	// Note: Initialization will fail if the required Attribute Set is not found!
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bListenForAttributeChanges = true;

	// Should we subscribe for effect events (add / remove etc)
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bListenForEffectEvents = false;

	// Only Effect with those tags will be forwarded and listened too.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "bListenForEffectEvents"))
	FGameplayTagRequirements EffectEventTagRequirements;

	UFUNCTION(BlueprintImplementableEvent)
	void K2_InitializeAbilitySystemWidget(const bool bBindingDone);

	// Called to initialize the User Widget and bind to Attribute change delegates
	// Can be called again to re-initialize the values
	UFUNCTION(BlueprintCallable, Category = "Ability System")
	bool InitializeAbilitySystemWidget(UAbilitySystemComponent* InOwnerAbilitySystemComponent);

	// Returns the Owner's Ability System Component.  
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UMKAbilitySystemComponent* GetOwnerAbilitySystemComponent() const;

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
	TWeakObjectPtr<UMKAbilitySystemComponent> AbilitySystemComponent;

	FDelegateHandle MaximumHealthChangeDelegate;
	FDelegateHandle MaximumStaminaChangeDelegate;
	FDelegateHandle MaximumManaChangeDelegate;
	FDelegateHandle MaximumFocusChangeDelegate;

	FDelegateHandle CurrentHealthChangeDelegate;
	FDelegateHandle CurrentStaminaChangeDelegate;
	FDelegateHandle CurrentManaChangeDelegate;
	FDelegateHandle CurrentFocusChangeDelegate;

	FDelegateHandle HealthRegenerationChangeDelegate;
	FDelegateHandle StaminaRegenerationChangeDelegate;

	FDelegateHandle ResistanceChangeDelegate;

	FDelegateHandle BleedingChangeDelegate;
	FDelegateHandle BleedHealChangeDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnBossEncounter OnBossEncounter;

	void MaximumHealthChanged(const FOnAttributeChangeData& Data);
	void MaximumStaminaChanged(const FOnAttributeChangeData& Data);
	void MaximumManaChanged(const FOnAttributeChangeData& Data);
	void MaximumFocusChanged(const FOnAttributeChangeData& Data);

	void CurrentHealthChanged(const FOnAttributeChangeData& Data);
	void CurrentStaminaChanged(const FOnAttributeChangeData& Data);
	void CurrentManaChanged(const FOnAttributeChangeData& Data);
	void CurrentFocusChanged(const FOnAttributeChangeData& Data);

	void HealthRegenerationChanged(const FOnAttributeChangeData& Data);
	void StaminaRegenerationChanged(const FOnAttributeChangeData& Data);

	void ResistanceChanged(const FOnAttributeChangeData& Data);

	void BleedingChanged(const FOnAttributeChangeData& Data);

	static void ResetDelegateHandle(FDelegateHandle DelegateHandle, UAbilitySystemComponent* OldAbilitySystemComponent,
									const FGameplayAttribute& Attribute);

	UFUNCTION()
	void OnGameplayEffectEventCallback(const FActiveGameplayEffect& Effect, const EASEffectEventType EventType);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnGameplayEffectEventCallback(const UMKAbilitySystemComponent* const ASC,
										  const EASEffectEventType EventType, const FActiveGameplayEffectHandle Handle,
										  const FMKEffectEventInfo Flags);
};
