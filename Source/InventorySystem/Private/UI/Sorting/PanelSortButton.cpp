// CopyRight KGCA - Team RedCoke


#include "UI/Sorting/PanelSortButton.h"
#include "CommonBorder.h"
#include "CommonTextBlock.h"
#include "Components/Button.h"

void UPanelSortButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	Title_Text->SetText(Title);
}

void UPanelSortButton::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnHovered.AddUniqueDynamic(this, &ThisClass::SetBorderColorWhite);
	Button->OnUnhovered.AddUniqueDynamic(this, &ThisClass::SetBorderColorTransparent);

	Button->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClickedEvent);
}

void UPanelSortButton::SetBorderColorWhite()
{
	Border->SetBrushColor(FLinearColor::White);
}

void UPanelSortButton::SetBorderColorTransparent()
{
	Border->SetBrushColor(FLinearColor::Transparent);
}

void UPanelSortButton::OnClickedEvent()
{
	OnButtonClicked.Broadcast(SortMethod);
}
