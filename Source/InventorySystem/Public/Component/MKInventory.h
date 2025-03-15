// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "InventorySystemComponent.h"
#include "MKInventory.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UMKInventory : public UInventorySystemComponent
{
	GENERATED_BODY()

public:
	UMKInventory();

protected:
	virtual void BeginPlay() override;

public:
	void OpenInventoryWidget();
	void CloseInventoryWidget();
	void ToggleInventoryMenu();

};
