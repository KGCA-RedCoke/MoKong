// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "PlayerHUDWidget.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDAttackWidget.generated.h"

class UEffectWidget_Skill;
/**
 * 
 */
UCLASS()
class MOKONG_API UPlayerHUDAttackWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnSkillSlotCooldown(const EASEffectEventType EventType, const FActiveGameplayEffect& Effect);

protected:
	UFUNCTION(BlueprintCallable, Category = "Ability System")
	bool InitializeAbilitySystemWidget(UAbilitySystemComponent* InOwnerAbilitySystemComponent);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_InitializeAbilitySystemWidget(const bool bBindingDone);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UMKAbilitySystemComponent* GetOwnerAbilitySystemComponent() const;

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnGameplayEffectEventCallback(const UMKAbilitySystemComponent* const ASC,
										  const EASEffectEventType EventType, const FActiveGameplayEffectHandle Handle,
										  const FMKEffectEventInfo Flags);

protected:
	TWeakObjectPtr<UMKAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<UEffectWidget_Skill> Slot_1;	//	정지술
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<UEffectWidget_Skill> Slot_2;	// 기화술
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<UEffectWidget_Skill> Slot_3;	// 변신
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<UEffectWidget_Skill> Slot_4;	// 분신술
};
