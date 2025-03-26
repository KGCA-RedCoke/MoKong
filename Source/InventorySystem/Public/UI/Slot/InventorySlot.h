// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ItemTypes.h"
#include "InventorySlot.generated.h"

class UTextBlock;
class UBorder;
class UImage;
class UButton;
class UInventoryWidget;
class UInventorySystemComponent;


/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventorySlot : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
#pragma region Gamepad
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
#pragma endregion

#pragma region Mouse
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
#pragma endregion

public:
	/* 위젯 초기화 */
	void InitializeWidget(UInventorySystemComponent* Owner, UInventoryWidget* ParentWidget,
						  int32                      Index);

	/* 슬롯의 데이터를 덮어쓰기 */
	UFUNCTION(BlueprintCallable)
	void OverwriteSlot(const FMKInventoryItemSpec& NewSlotData);

	/* 슬롯의 초기화가 완료되었을 때 이 함수를 호출 */
	void SetInitialInputDelay(const bool bValue);

	void HighlightSlot();

private:
	/* 아이템 데이터에 따라 버튼 이미지 설정 */
	void SetButtonStyle(const FMKInventoryItemSpec& InItemData) const;
	/* 아이템 데이터에 따른 수량 텍스트 설정 */
	void SetItemQuantity() const;
	/* 플레이어의 활성화된 위젯 반환 */
	EItemCategory GetActiveWidget() const;
	/* 현재 열려진 팝업이 있는지 확인 */
	bool IsAnyPopupActive() const;

#pragma region Click Event
	/* 처음 생성되고 1초의 딜레이 이후 RightClick 실행 */
	UFUNCTION()
	void DelayedClick();
	/* 이 슬롯에서 포커스가 왔을 때 처리 */
	UFUNCTION()
	void OnItemButtonHovered();
	/* 이 슬롯에서 포커스가 나갔을 때 처리 */
	UFUNCTION()
	void OnItemButtonUnHovered();
	/* 오른쪽 마우스버튼 이벤트 */
	void OnRightClick();

	/* 클릭 됐을 때 인벤토리 처리 */
	void InventoryWindow();

#pragma endregion

	/** 아이템이 사용가능한 아이템이면 사용시도 */
	void TryToUseAnItem();

protected:
#pragma region Reference
	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Slot")
	TObjectPtr<UInventorySystemComponent> OwnerInventory;
	UPROPERTY(BlueprintReadOnly, Category = "Inventory|Slot")
	TObjectPtr<UInventoryWidget> ParentWidgetRef;
#pragma endregion

#pragma region Widget
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> Item_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Item_Image;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Quantity_Text;
#pragma endregion

#pragma region Animation
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HighlightAnim;
#pragma endregion

#pragma region Data
	/* 초기화가 완전히 되기 전까지 false */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Slot|Setting")
	bool bInitialInputDelay = false;
	/* 마우스가 올려졌을때의 색상 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Slot|Setting")
	FLinearColor BorderHovered_Color = FLinearColor(0.55, 0.25, 0, 1);
	/* 마우스가 내려졌을때의 색상 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Slot|Setting")
	FLinearColor BorderUnHovered_Color = FLinearColor(0.27, 0.27, 0.27, 1);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Slot|Setting")
	TMap<EItemRarity, TObjectPtr<UTexture2D>> RarityTextures;
	//* 아이템 희귀도에 따른 슬롯의 테두리 색상 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Slot|Setting")
	TMap<EItemRarity, TObjectPtr<UTexture2D>> RarityTextures2;
	//* 슬롯의 아이템이 없을때의 텍스쳐 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Slot|Setting")
	TObjectPtr<UTexture2D> EmptySlotTexture;

	/* 이 슬롯 아이템의 데이터 */
	UPROPERTY(BlueprintReadWrite, Category = "Inventory|Slot")
	FMKInventoryItemSpec SlotItemSpec;
#pragma endregion
};
