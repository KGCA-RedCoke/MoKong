// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "InventorySystemComponent.h"
#include "MKEquipmentInventory.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UMKEquipmentInventory : public UInventorySystemComponent
{
	GENERATED_BODY()

public:
	UMKEquipmentInventory();

public:
	virtual void InitializeInventory(APlayerController* PlayerController) override;
	virtual bool HandleItemUse(const EInventoryPanel Panel, const int SlotIndex) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory|Equipment")
	void InitializeEquipmentInventory(APawn* PlayerPawn);

public:
	UFUNCTION(BlueprintCallable)
	bool EquipItem(const FMKInventoryItemSpec& ItemData, int32 SlotIndex);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Equipment")
	TMap<EInventoryPanel, FDataTableRowHandle> SavedEquipmentData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Equipment")
	TMap<EInventoryPanel, FMKInventoryItemSpec> EquippedItems;
};
