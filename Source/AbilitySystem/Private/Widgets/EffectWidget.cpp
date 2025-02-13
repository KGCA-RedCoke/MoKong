// CopyRight KGCA - Team RedCoke


#include "Widgets/EffectWidget.h"

void UEffectWidget::UpdateTitle(const FText NewTitle)
{
	Text_Title->SetText(NewTitle);
	if (RB_Retainer)
	{
		RB_Retainer->RequestRender();
	}
}

void UEffectWidget::UpdateDescription(const FText NewDescription)
{
	Text_Description->SetText(NewDescription);
	if (RB_Retainer)
	{
		RB_Retainer->RequestRender();
	}
}

void UEffectWidget::UpdateIcon(TSoftObjectPtr<UTexture2D> NewSoftTexture, TSoftObjectPtr<UMaterialInterface> SoftMaterial)
{
	if (!SoftMaterial.IsNull())
	{
		Image_Icon->SetBrushFromSoftMaterial(SoftMaterial);
	}
	else if (!NewSoftTexture.IsNull())
	{
		Image_Icon->SetBrushFromSoftTexture(NewSoftTexture);
	}

	if (RB_Retainer)
	{
		RB_Retainer->RequestRender();
	}
}

void UEffectWidget::UpdateStackCount(const int32 NewStackCount)
{
	Text_StackCount->SetCurrentValue(NewStackCount);
	Text_StackCount->SetVisibility(NewStackCount > 1 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	if (RB_Retainer)
	{
		RB_Retainer->RequestRender();
	}
}

void UEffectWidget::UpdateDuration(const float NewDuration)
{
	if (!GetWorld())
	{
		return;
	}

	T_Duration->SetCurrentValue(NewDuration);
	ESlateVisibility NewVisibility = ESlateVisibility::Collapsed;

	if (NewDuration > 0.f)
	{
		NewVisibility = ESlateVisibility::SelfHitTestInvisible;
	}

	T_Duration->SetVisibility(NewVisibility);
	if (RB_Retainer)
	{
		RB_Retainer->RequestRender();
	}
}
