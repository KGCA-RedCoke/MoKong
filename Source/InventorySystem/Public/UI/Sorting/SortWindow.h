// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ItemTypes.h"
#include "SortWindow.generated.h"

class UInventorySystemComponent;
class UPanelSortButton;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndSorting);


/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API USortWindow : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	FOnEndSorting OnEndSorting;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	void ToggleSortingWindow();

	UFUNCTION()
	void OnSort(const ESortMethod SortMethod);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelSortButton> SortByTypeButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelSortButton> SortByRarityButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelSortButton> SortByValueButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelSortButton> SortByWeightButton;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory|SortWindow")
	TObjectPtr<UInventorySystemComponent> OwnerInventory;
};
