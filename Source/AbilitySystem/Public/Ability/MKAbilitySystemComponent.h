// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Actor/MKAbilityPlayer.h"
#include "MKAbilitySystemComponent.generated.h"

class UMKGameplayAbility;
class AMKAbilityCharacter;

UENUM(BlueprintType)
enum class EASEffectEventType : uint8
{
	Added,
	Removed
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameplayEffectEventDelegate,
											 const FActiveGameplayEffect&,
											 Effect,
											 EASEffectEventType,
											 EventType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate,
											   UMKAbilitySystemComponent*,
											   SourceASC,
											   float,
											   UnmitigatedDamage,
											   float,
											   MitigatedDamage);

/**
 * UAbilitySystemComponent의 확장
 * - 기본적으로 액터와 GAS시스템 사이의 인터페이스 역할을 한다.
 * - GAS시스템과 상호작용해야하는 액터는 이 컴포넌트를 가지고 있거나 다른 액터가 소유하고 있는 컴포넌트를 액세스 할 수 있어야 한다.
 * ----> 그러니까 어떠한 액터가 GAS를 쓰려면 이 컴포넌트를 가지고 있어야 한다!!!
 * -------------- 요구사항 --------------
 * - Actor가 IAbilitySystemInterface 인터페이스를 구현 및 GetAbilitySystemComponent 함수를 구현해야 한다.
 * - 액터가 소멸되어 리스폰될 수 있는 경우에는 특히 어빌리티 시스템 컴포넌트를 플레이어 스테이트 같은 곳에 보관하는 것이 좋다.
 */
UCLASS(ClassGroup=("AbilitySystem"))
class ABILITYSYSTEM_API UMKAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UMKAbilitySystemComponent();

	UFUNCTION(BlueprintCallable)
	void InitializeAbilitySystemData(const FAbilitySystemInitializationData& AbilitySystemInitializationData,
									 AActor*                                 InOwningActor, AActor* InAvatarActor);

	UFUNCTION()
	void TryActivateAbilityByName(FName AbilityName);

	const UAttributeSet* GetOrCreateAttributeSet(const TSubclassOf<UAttributeSet>& InAttributeSet);

	AMKAbilityCharacter*              GetAbilityCharacter() const;
	static UMKAbilitySystemComponent* GetAbilitySystemComponentFromActor
	(const AActor* Actor, bool LookForComponent = false);

	void GetActiveAbilitiesWithTags(const FGameplayTagContainer&        GameplayTagContainer,
									TArray<UMKAbilitySystemComponent*>& ActiveAbilities) const;

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

	FReceivedDamageDelegate ReceivedDamage;

	virtual void ReceiveDamage(UMKAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);

	void AddSpecHandle(FName AbilityName, const FGameplayAbilitySpecHandle& Handle);
	
	bool bStartupEffectsApplied;
	bool AbilitySystemDataInitialized;

protected:
	// Callback functions InputPressed, InputReleased, InputHeld
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	// Callback functions where a GE is added, inhibited or removed.
	virtual void OnGameplayEffectAddedCallback(UAbilitySystemComponent* const ASC, const FGameplayEffectSpec& Spec,
											   FActiveGameplayEffectHandle    Handle);
	virtual void OnGameplayEffectRemovedCallback(const FActiveGameplayEffect& ActiveGameplayEffect);

protected:
	// Handles to abilities that had their input pressed this frame.
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	// Handles to abilities that have their input held.
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;

	TMap<FName, FGameplayAbilitySpecHandle> AbilityHandles;

public:
	FOnGameplayEffectEventDelegate OnMKGameplayEffectEventDelegate;
};
