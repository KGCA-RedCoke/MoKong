// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "CommonLazyImage.h"
#include "CommonNumericTextBlock.h"
#include "CommonTextBlock.h"
#include "CommonUserWidget.h"
#include "Components/RetainerBox.h"
#include "EffectWidget.generated.h"

class UCommonLazyImage;
class UCommonTextBlock;

/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API UEffectWidget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	FTimerHandle DurationUpdateTimer;

	float RemainingDuration = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidgetOptional))
	TObjectPtr<URetainerBox> RB_Retainer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Text_Title;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Text_Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> Image_Icon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidgetOptional))
	TObjectPtr<UCommonNumericTextBlock> Text_StackCount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidgetOptional))
	TObjectPtr<UCommonNumericTextBlock> T_Duration;

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
};
