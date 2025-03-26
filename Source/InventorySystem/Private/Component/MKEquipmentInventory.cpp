// CopyRight KGCA - Team RedCoke


#include "Component/MKEquipmentInventory.h"


UMKEquipmentInventory::UMKEquipmentInventory()
{
	InitialInventoryPanelCount = 12;

	PanelsToUse.Append({
						   EInventoryPanel::Weapon,
						   EInventoryPanel::Accessory,
						   EInventoryPanel::HonBaek,
						   EInventoryPanel::Helmet,
						   EInventoryPanel::Gloves,
						   EInventoryPanel::Suit,
						   EInventoryPanel::Shoes
					   });
}

void UMKEquipmentInventory::InitializeInventory(APlayerController* PlayerController)
{
	Super::InitializeInventory(PlayerController);
}

bool UMKEquipmentInventory::HandleItemUse(const EInventoryPanel Panel, const int SlotIndex)
{


	return true;
}

void UMKEquipmentInventory::InitializeEquipmentInventory_Implementation(APawn* PlayerPawn)
{
	for (const auto& Element : SavedEquipmentData)
	{}
}

bool UMKEquipmentInventory::EquipItem(const FMKInventoryItemSpec& ItemData, int32 SlotIndex)
{
	return false;
}


void UMKEquipmentInventory::BeginPlay()
{
	Super::BeginPlay();

}
