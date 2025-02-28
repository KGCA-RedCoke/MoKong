// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Ability/AbilityBindWidgetBase.h"
#include "PlayerHUDAttackWidget.generated.h"

class UEffectWidget_Skill;
/**
 * 
 */
UCLASS()
class MOKONG_API UPlayerHUDAttackWidget : public UAbilityBindWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void EffectChangeCallback(const EASEffectEventType EventType, const FActiveGameplayEffect& Effect) override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<UEffectWidget_Skill> Slot_1;	//	정지술
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<UEffectWidget_Skill> Slot_2;	// 기화술
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<UEffectWidget_Skill> Slot_3;	// 변신
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<UEffectWidget_Skill> Slot_4;	// 분신술
};
