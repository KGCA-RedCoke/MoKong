// CopyRight KGCA - Team RedCoke


#include "UI/Sorting/SortWindow.h"

#include "Component/InventorySystemComponent.h"
#include "UI/Sorting/PanelSortButton.h"

void USortWindow::NativeConstruct()
{
	Super::NativeConstruct();


	SetIsFocusable(true);

	OwnerInventory = Cast<UInventorySystemComponent>(GetOwningPlayer()->GetComponentByClass(UInventorySystemComponent::StaticClass()));
	check(OwnerInventory)

	SortByTypeButton->OnButtonClicked.AddUniqueDynamic(this, &ThisClass::OnSort);
	SortByRarityButton->OnButtonClicked.AddUniqueDynamic(this, &ThisClass::OnSort);
	SortByValueButton->OnButtonClicked.AddUniqueDynamic(this, &ThisClass::OnSort);
	SortByWeightButton->OnButtonClicked.AddUniqueDynamic(this, &ThisClass::OnSort);
}

void USortWindow::NativeDestruct()
{
	Super::NativeDestruct();

	SortByTypeButton->OnButtonClicked.RemoveDynamic(this, &ThisClass::OnSort);
	SortByRarityButton->OnButtonClicked.RemoveDynamic(this, &ThisClass::OnSort);
	SortByValueButton->OnButtonClicked.RemoveDynamic(this, &ThisClass::OnSort);
	SortByWeightButton->OnButtonClicked.RemoveDynamic(this, &ThisClass::OnSort);
}

FReply USortWindow::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void USortWindow::ToggleSortingWindow() {}

void USortWindow::OnSort(const ESortMethod SortMethod)
{
	OwnerInventory->SortInventory(SortMethod, OwnerInventory->GetActivePanel(), false);

	OnEndSorting.Broadcast();
}
