// CopyRight KGCA - Team RedCoke


#include "UI/Panel/InventoryPanelButton.h"

#include "Components/Border.h"
#include "Components/Button.h"

void UInventoryPanelButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	Button->SetStyle(Off_ButtonStyle);
}

void UInventoryPanelButton::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddUniqueDynamic(this, &ThisClass::OnClickedEvent);
}

void UInventoryPanelButton::RefreshPanelButton(const EInventoryPanel ActivePanel) const
{
	Border->SetBrushColor(ActivePanel == Panel ? FLinearColor::White : FLinearColor::Transparent);
	Button->SetStyle(ActivePanel == Panel ? On_ButtonStyle : Off_ButtonStyle);
}

void UInventoryPanelButton::OnClickedEvent()
{
	OnClickedButtonPanel.Broadcast(Panel);
}
