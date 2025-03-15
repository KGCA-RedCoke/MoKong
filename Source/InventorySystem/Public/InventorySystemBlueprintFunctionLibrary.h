// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventorySystemBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventorySystemBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 인벤토리 패널을 아이템에 맞게 반환
	 * @param ItemData 아이템 데이터
	 * @return 인벤토리 패널
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static EInventoryPanel GetInventoryPanelFromItem(const FMKItemSpec& ItemData);

};
