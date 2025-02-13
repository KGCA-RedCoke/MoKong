// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class UTextBlock;
class UOverlay;
class UHorizontalBox;
class UGridPanel;
class UBorder;
class USizeBox;
class UWidgetSwitcher;
class UInteractionTargetComponent;
/**
 * 
 */
UCLASS()
class UISYSTEM_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	bool IsOnScreen(const FMargin& InScreenMargin);

	void ToggleLootText(bool bShow);
	void ClearWidgetData();
	void ResetProgress();

	void UpdateInteractionText();
	void UpdatePositionInViewport();
	void UpdateInteractionProgress(float NewProgress);
	void UpdateBrushFromTarget();
	void UpdateControllerScreenSettings(UInteractionTargetComponent* TargetComponent);

	void PlayFlashingAnimation();

private:
#pragma region Widget

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UWidgetSwitcher> ContentSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<USizeBox> POI_Container;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UBorder> POI_Icon;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UGridPanel> InteractionContainer;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UBorder> Icon;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UHorizontalBox> InteractionInfoBox;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UBorder> Background;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UBorder> Foreground;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UOverlay> TriangleBox;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UBorder> Triangle;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UWidgetSwitcher> LootTextSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UBorder> LootKeyIcon;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextBlock> LootKey;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UTextBlock> InfoText;

#pragma endregion Widget

#pragma region Widget Animation

	FWidgetAnimationDynamicEvent SlideUpStartDelegate;
	FWidgetAnimationDynamicEvent SlideUpEndDelegate;

	UPROPERTY(BlueprintReadOnly, Category = "Animation", Transient, meta = (BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> SlideUp;

	UPROPERTY(BlueprintReadOnly, Category = "Animation", Transient, meta = (BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> LootText;

	UPROPERTY(BlueprintReadOnly, Category = "Animation", Transient, meta = (BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> Flash;

	UPROPERTY(BlueprintReadOnly, Category = "Animation", Transient, meta = (BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> InteractionCompleted;

	UPROPERTY(BlueprintReadOnly, Category = "Animation", Transient, meta = (BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> InteractionCanceled;

	UPROPERTY(BlueprintReadOnly, Category = "Animation", Transient, meta = (BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> InteractionRepeatUpdate;

	UPROPERTY(BlueprintReadOnly, Category = "Animation", Transient, meta = (BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> Rotate;

	UPROPERTY(BlueprintReadOnly, Category = "Animation", Transient, meta = (BindWidgetAnim, AllowPrivateAccess))
	TObjectPtr<UWidgetAnimation> InteractionTextAnim;

#pragma endregion Widget Animation

#pragma region Component

	UPROPERTY(BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess))
	TObjectPtr<UInteractionTargetComponent> WidgetInteractionTarget;

#pragma endregion Component

#pragma region Property

	UPROPERTY(BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess))
	bool bIsInteractionState;

	UPROPERTY(BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess))
	bool bShowProgress;

	UPROPERTY(BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess))
	bool bClearTarget;

	UPROPERTY(BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess))
	bool bCustomClockWise;

	UPROPERTY(BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess))
	float CustomProgress;

	UPROPERTY(BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess))
	float CurrentProgress;

	UPROPERTY(BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess))
	float ScreenRadiusProgress;

	UPROPERTY(BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess))
	FVector LastWorldLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess))
	FMargin ScreenMargin;

	UPROPERTY(BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess))
	FVector2D WidgetPositionErrorFactor;

	UPROPERTY(BlueprintReadWrite, Category = "Property", meta = (AllowPrivateAccess))
	FText CurrentInterationText;

#pragma endregion Property
};
