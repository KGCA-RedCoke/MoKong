// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Blueprint/UserWidget.h"
#include "AbilityBindWidgetBase.generated.h"

class UMKAbilitySystemComponent;

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


/**
 * 
 */
UCLASS()
class MOKONG_API UAbilityBindWidgetBase : public UUserWidget
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
	virtual bool InitializeAbilitySystemWidget(UAbilitySystemComponent* OwnerAbilitySystemComponent);

	// Returns the Owner's Ability System Component.  
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UMKAbilitySystemComponent* GetOwnerAbilitySystemComponent() const;


	static void ResetDelegateHandle(FDelegateHandle DelegateHandle, UAbilitySystemComponent* OldAbilitySystemComponent,
									const FGameplayAttribute& Attribute);

protected:
	TWeakObjectPtr<UMKAbilitySystemComponent> AbilitySystemComponent;

public:
	UFUNCTION()
	virtual void EffectChangeCallback(const EASEffectEventType EventType, const FActiveGameplayEffect& Effect);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnGameplayEffectEventCallback(const UMKAbilitySystemComponent* const ASC,
										  const EASEffectEventType EventType, const FActiveGameplayEffectHandle Handle,
										  const FMKEffectEventInfo Flags);
};
