// CopyRight KGCA - Team RedCoke


#include "InteractionSystem/UI/InteractionWidget.h"
#include "Kismet/GameplayStatics.h"

bool UInteractionWidget::IsOnScreen(const FMargin& InScreenMargin)
{
	FVector2D ScreenPosition;
	UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), LastWorldLocation, ScreenPosition, true);


	return false;
}

void UInteractionWidget::ToggleLootText(bool bShow) {}
void UInteractionWidget::ClearWidgetData() {}
void UInteractionWidget::ResetProgress() {}
void UInteractionWidget::UpdateInteractionText() {}

void UInteractionWidget::UpdatePositionInViewport()
{
	// GetOwningPlayer()->GetViewportSize()
	// // 스크린 정중앙 좌표를 월드 좌표로 변환
	// FVector2D ScreenPosition = GetSize
	
}

void UInteractionWidget::UpdateInteractionProgress(float NewProgress) {}
void UInteractionWidget::UpdateBrushFromTarget() {}
void UInteractionWidget::UpdateControllerScreenSettings(UInteractionTargetComponent* TargetComponent) {}
void UInteractionWidget::PlayFlashingAnimation() {}
