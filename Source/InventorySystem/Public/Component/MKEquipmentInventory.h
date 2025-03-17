// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "InventorySystemComponent.h"
#include "MKEquipmentInventory.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UMKEquipmentInventory : public UInventorySystemComponent
{			
	GENERATED_BODY()

public:
	UMKEquipmentInventory();

public:
	UFUNCTION(BlueprintCallable)
	bool EquipItem(const FMKInventoryItemSpec& ItemData, int32 SlotIndex);

protected:
	virtual void BeginPlay() override;

public:
	void OpenInventoryWidget();
	void CloseInventoryWidget();
	void ToggleInventoryMenu();

};
