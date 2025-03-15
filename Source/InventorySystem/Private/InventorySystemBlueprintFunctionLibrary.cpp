// CopyRight KGCA - Team RedCoke


#include "InventorySystemBlueprintFunctionLibrary.h"

EInventoryPanel UInventorySystemBlueprintFunctionLibrary::GetInventoryPanelFromItem(const FMKItemSpec& ItemData)
{
	return ItemData.Panel;
}
