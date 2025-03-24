// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.h"
#include "UObject/Interface.h"
#include "ModularCharacter.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UModularCharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYSYSTEM_API IModularCharacter
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory|Modular")
	void EquipItem(const FMKInventoryItemSpec& ItemData);


};
