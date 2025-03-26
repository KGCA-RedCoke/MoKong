// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ItemTypes.h"
#include "InventoryWidget.generated.h"

class UHorizontalBox;
class UInventoryPanelButton;
class UInventorySlot;
class UImage;
class UBorder;
class USortWindow;
class UWidgetSwitcher;
class UScrollBox;
class UInventorySystemComponent;
class UUniformGridPanel;

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventoryWidget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

private:
	/* 인벤토리 슬롯을 설정하는 함수
	 * 패널별로 인벤토리를 생성
	 * @param PanelToCreate 생성할 패널
	 */
	UFUNCTION(BlueprintCallable)
	void CreateInventoryPanel(const EInventoryPanel PanelToCreate);

	/**
	 * 슬롯을 하나씩 생성하고 그리드패널에 집어넣음 
	 * @param Inventory 생성될 인벤토리
	 * @param InventorySize 인벤토리 사이즈
	 * @param UniformGridPanel 그리드 패널
	 */
	void SetInventorySlots(const TArray<FMKInventoryItemSpec>& Inventory, int32 InventorySize,
						   UUniformGridPanel*                  UniformGridPanel);


	void DisplaySample(int32 NumToDisplay);

	EInventoryPanel               GetActivePanel() const;
	TObjectPtr<UUniformGridPanel> GetUniformGridFromPanel(const EInventoryPanel Panel) const;

	void SetActivePanel();
	void SetPanelTitle(EInventoryPanel Panel) const;
	void SetFocusToSlot(int32 SlotIndex);
	void ResetSlotFocus();

	#pragma region Callback
	UFUNCTION()
	void RefreshInventory(const EInventoryPanel Panel);
	UFUNCTION()
	void HighlightSlot(const int SlotIndex);
	UFUNCTION()
	void SwitchInventoryPanel(const EInventoryPanel NewPanel);
	UFUNCTION()
	void OnSortingFinished();
#pragma endregion

protected:
#pragma region Reference
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventorySystemComponent> ReceiverInventory;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventorySystemComponent> OwnerInventory;
#pragma endregion

#pragma region Widget
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> Panel_ButtonsHolder;
	UPROPERTY(BlueprintReadWrite)
	TMap<EInventoryPanel, TObjectPtr<UInventoryPanelButton>> PanelButtons;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UWidgetSwitcher> Panel_WidgetSwitcher;
	UPROPERTY(BlueprintReadWrite)
	TMap<EInventoryPanel, TObjectPtr<UUniformGridPanel>> InventoryPanels;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCommonTextBlock> PanelName;

	/*UPROPERTY(meta = (BindWidget))
	TObjectPtr<USortWindow> SortWindow;*/
#pragma endregion

#pragma region Animation
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn;
#pragma endregion

#pragma region Data
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 RowLength = 4;
	UPROPERTY()
	int32 FocusedSlot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 SampleSlotsToDisplay = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInventoryPanel ActivePanel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D DraggedImageSize;
#pragma endregion

#pragma region Setting
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Inventory|Setting")
	TSubclassOf<UInventorySlot> InventorySlotClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Inventory|Setting")
	USoundCue* SwitchSoundCue;
#pragma endregion
};
