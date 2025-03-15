// CopyRight KGCA - Team RedCoke


#include "Component/InventorySystemComponent.h"
#include "InventorySystemBlueprintFunctionLibrary.h"


UInventorySystemComponent::UInventorySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventorySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventorySystemComponent::InitializeInventory(APlayerController* PlayerController)
{
	// 서버에서 실행되지 않는 경우 리턴
	if (!PlayerController->HasAuthority())
		return;

	PlayerControllerRef = PlayerController;

	for (const auto& Panel : PanelsToUse)
	{
		InventoryPanels.Add(Panel);
		InitializePanel(Panel);
	}
}

void UInventorySystemComponent::InitializePanel(EInventoryPanel Panel)
{
	InventoryPanels[Panel].SetNum(16);

	for (int i = 0; i < InventoryPanels[Panel].Num(); ++i)
	{
		InventoryPanels[Panel][i].Index = i;
	}
}

void UInventorySystemComponent::AddItemToInventoryArray(const FMKItemSpec& ItemSpec, int SlotIndex)
{
	if (!PlayerControllerRef->HasAuthority())
	{
		return;
	}

	const EInventoryPanel PanelToInsert =
			UInventorySystemBlueprintFunctionLibrary::GetInventoryPanelFromItem(ItemSpec);

	if (auto* Array = GetInventoryArray(PanelToInsert))
	{
		if (SlotIndex >= 0)
		{
			FMKInventoryItemSpec NewItem;
			NewItem.ItemData = ItemSpec;
			NewItem.Quantity = 1;
			NewItem.Index    = SlotIndex;

			if (Array->Num() <= SlotIndex && (*Array)[SlotIndex].Quantity != 0)
			{
				Array->Add(NewItem);
			}
			else
			{
				(*Array)[SlotIndex] = NewItem;
			}

			SwitchActivePanel(PanelToInsert);

			OnRefreshInventory.Broadcast(PanelToInsert);
			OnAddedToInventories.Broadcast(NewItem);
			OnHighlightInventorySlot.Broadcast(SlotIndex);
		}
	}
}


void UInventorySystemComponent::AddToStackInInventory(const FMKItemSpec& ItemSpec, int32 SlotIndex)
{
	if (!PlayerControllerRef->HasAuthority())
	{
		return;
	}

	FMKInventoryItemSpec* AlreadyInventory =
			GetItemBySlot(UInventorySystemBlueprintFunctionLibrary::GetInventoryPanelFromItem(ItemSpec),
						  SlotIndex);
	if (AlreadyInventory->ItemData == ItemSpec)
	{
		AlreadyInventory->Quantity++;
	}
	else
	{
		AddItemToInventoryArray(ItemSpec, SlotIndex);
	}
}

void UInventorySystemComponent::RemoveItemFromPanel(const EInventoryPanel Panel, const int SlotIndex, bool bRemoveAll)
{
	if (PlayerControllerRef->HasAuthority())
	{
		return;
	}

	if (auto* Array = GetInventoryArray(Panel))
	{
		(*Array)[SlotIndex].Quantity--;

		if (bRemoveAll)
		{
			(*Array)[SlotIndex].ItemData = FMKItemSpec();
			(*Array)[SlotIndex].Quantity = 0;
		}

		OnRefreshInventory.Broadcast(Panel);
	}
}

void UInventorySystemComponent::RemoveItemFromInventoryArray(const FMKInventoryItemSpec& InItemData)
{
	if (PlayerControllerRef->HasAuthority())
	{
		return;
	}

	// 아이템이 속해있는 패널
	const EInventoryPanel ItemPanel =
			UInventorySystemBlueprintFunctionLibrary::GetInventoryPanelFromItem(InItemData.ItemData);

	// 아이템이 속해있는 패널의 배열을 가져옴
	if (auto* Array = GetInventoryArray(ItemPanel))
	{
		(*Array)[InItemData.Index].ItemData = FMKItemSpec();
		(*Array)[InItemData.Index].Quantity = 0;

		OnRemovedFromInventories.Broadcast(InItemData);
		OnRefreshInventory.Broadcast(ItemPanel);
	}

}

void UInventorySystemComponent::SwitchActivePanel(const EInventoryPanel InventoryPanel)
{
	if (InventoryPanel != ActivePanel && InventoryPanels.Contains(InventoryPanel))
	{
		ActivePanel = InventoryPanel;

		OnSwitchedActivePanel.Broadcast(ActivePanel);
	}
}

void UInventorySystemComponent::SortInventory(ESortMethod SortMethod, EInventoryPanel SinglePanel, bool bSortAllPanel)
{
	if (bSortAllPanel)
	{
		for (auto& Panel : InventoryPanels)
		{
			TArray<FMKInventoryItemSpec>& SortedInventory = Panel.Value;

			switch (SortMethod)
			{
			case ESortMethod::SimpleSort:
				/*SortSimple(Panel.Value, SortedInventory);*/
				break;
			case ESortMethod::By_Type:
				UE_LOG(LogTemp, Warning, TEXT("SortByType is not implemented yet"));
			/*SortByType(SortedInventory);*/
				break;
			case ESortMethod::By_Rarity:
				SortByRarity(SortedInventory);
				break;
			}

			InitializePanel(Panel.Key);

			OnRefreshInventory.Broadcast(Panel.Key);
		}
	}
	else
	{
		if (auto* Array = GetInventoryArray(SinglePanel))
		{
			TArray<FMKInventoryItemSpec>& SortedInventory = *Array;
			switch (SortMethod)
			{
			case ESortMethod::SimpleSort:
				SortSimple(*Array, SortedInventory);
				break;
			case ESortMethod::By_Type:
				UE_LOG(LogTemp, Warning, TEXT("SortByType is not implemented yet"));
			/*SortByType(SortedInventory);*/
				break;
			case ESortMethod::By_Rarity:
				SortByRarity(SortedInventory);
				break;
			}


			InitializePanel(SinglePanel);

			OnRefreshInventory.Broadcast(SinglePanel);
		}
	}
}

void UInventorySystemComponent::SortSimple(TArray<FMKInventoryItemSpec>& Inventory,
										   TArray<FMKInventoryItemSpec>& SortedInventory)
{
	// FilterByPredicate는 TArray의 멤버함수로, Predicate를 만족하는 요소들로 이루어진 새로운 배열을 반환
	SortedInventory = Inventory.FilterByPredicate([](const FMKInventoryItemSpec& ItemData){
		return ItemData.Quantity > 0;
	});
}

/*void UInventorySystemComponent::SortByType(TArray<FMKInventoryItemSpec>& Inventory)
{
	Inventory.Sort([](const FMKInventoryItemSpec& Lhs, const FMKInventoryItemSpec& Rhs){
		if (Lhs.ItemClass)
		{
			if (Rhs.ItemClass)
			{
				return Lhs.ItemData.ItemName > Rhs.ItemData.ItemName;
			}
			return true;
		}
		return false;
	});
}*/


void UInventorySystemComponent::SortByRarity(TArray<FMKInventoryItemSpec>& Inventory)
{
	Inventory.Sort([](const FMKInventoryItemSpec& Lhs, const FMKInventoryItemSpec& Rhs){
		return Lhs.ItemData.Rarity < Rhs.ItemData.Rarity;
	});
}

FMKInventoryItemSpec* UInventorySystemComponent::GetItemBySlot(const EInventoryPanel Panel, const int SlotIndex)
{

	if (auto* Array = GetInventoryArray(Panel))
	{
		return Array->IsValidIndex(SlotIndex) ? &(*Array)[SlotIndex] : nullptr;
	}
	return nullptr;
}

TArray<FMKInventoryItemSpec>* UInventorySystemComponent::GetInventoryArray(const EInventoryPanel Panel)
{
	return InventoryPanels.Find(Panel);
}

EInventoryPanel UInventorySystemComponent::GetActivePanel() const
{
	return ActivePanel;
}
