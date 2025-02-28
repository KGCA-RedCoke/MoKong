// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EffectWidget_Skill.generated.h"

class UImage;
class UProgressBar;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API UEffectWidget_Skill : public UUserWidget
{
	GENERATED_BODY()

protected:
	FTimerHandle DurationUpdateTimer;

	float RemainingDuration = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TickInterval = 0.1f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<UProgressBar> ProgressBar_CoolTime;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateTitle(const FText NewTitle);

	UFUNCTION(BlueprintCallable)
	void UpdateDescription(const FText NewDescription);

	UFUNCTION(BlueprintCallable)
	void UpdateIcon(TSoftObjectPtr<UTexture2D> NewSoftTexture, TSoftObjectPtr<UMaterialInterface> SoftMaterial);

	UFUNCTION(BlueprintCallable)
	void UpdateStackCount(const int32 NewStackCount);

	UFUNCTION(BlueprintCallable)
	void UpdateDuration(const float NewDuration);

	void SetCooldown(const float Duration);

	void OnCooldownFinished();

	FTimerHandle CooldownTimerHandle;

	float CooldownDuration;
	float ElapsedTime;
};
