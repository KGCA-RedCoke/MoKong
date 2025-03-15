// CopyRight KGCA - Team RedCoke


#include "UI/Slot/InventorySlot.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventorySlot::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
}

void UInventorySlot::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
}

FReply UInventorySlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventorySlot::InitializeWidget(UInventorySystemComponent* Owner,
									  UInventoryWidget*          ParentWidget, int32 Index)
{
	SetIsFocusable(true);

	OwnerInventory  = Owner;
	ParentWidgetRef = ParentWidget;

	SetButtonStyle(SlotItemSpec);

	Item_Button->OnClicked.AddUniqueDynamic(this, &ThisClass::DelayedClick);
	Item_Button->OnHovered.AddUniqueDynamic(this, &ThisClass::OnItemButtonHovered);
	Item_Button->OnUnhovered.AddUniqueDynamic(this, &ThisClass::OnItemButtonUnHovered);
}

void UInventorySlot::OverwriteSlot(const FMKInventoryItemSpec& NewSlotData)
{
	SlotItemSpec = NewSlotData;

	SetButtonStyle(SlotItemSpec);

	SetItemQuantity();
}

void UInventorySlot::SetInitialInputDelay(const bool bValue)
{
	bInitialInputDelay = bValue;
}

void UInventorySlot::HighlightSlot()
{
	if (!HasUserFocusedDescendants(GetOwningPlayer()) && !IsHovered())
	{
		PlayAnimation(HighlightAnim);
	}
}

void UInventorySlot::SetButtonStyle(const FMKInventoryItemSpec& InItemData) const
{
	// Quantity가 0? 빈 슬롯으로 생각하겠음
	if (InItemData.Quantity <= 0)
	{
		// Item_Image->SetVisibility(ESlateVisibility::Collapsed);
		// Image_Border->SetVisibility(ESlateVisibility::Collapsed);
		Image_BorderStyle2->SetBrushFromTexture(EmptySlotTexture.Get());

		return;
	}

	if (UTexture2D* Thumbnail = InItemData.ItemData.IconThumbnail.LoadSynchronous())
	{
		Item_Image->SetBrushFromTexture(Thumbnail);
		Item_Image->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Image_Border->SetVisibility(ESlateVisibility::Visible);
	}

	if (RarityTextures.Contains(InItemData.ItemData.Rarity))
	{
		Image_Border->SetBrushFromTexture(RarityTextures[InItemData.ItemData.Rarity]);
		Image_BorderStyle2->SetBrushFromTexture(RarityTextures2[InItemData.ItemData.Rarity]);
	}
}

void UInventorySlot::SetItemQuantity() const
{
	if (SlotItemSpec.Quantity > 0)
	{
		if (SlotItemSpec.ItemData.bStackable && SlotItemSpec.Quantity > 1)
		{
			Quantity_Text->SetText(FText::AsNumber(SlotItemSpec.Quantity));
			return;
		}
	}

	Quantity_Text->SetText(FText::FromString(""));
	Quantity_Text->SetVisibility(ESlateVisibility::Hidden);
}

EItemCategory UInventorySlot::GetActiveWidget() const
{
	return {};
}

bool UInventorySlot::IsAnyPopupActive() const
{
	return false;
}

void UInventorySlot::DelayedClick()
{
	if (!bInitialInputDelay)
		OnRightClick();
}

void UInventorySlot::OnItemButtonHovered()
{
	if (!IsAnyPopupActive())
	{
		StopAnimation(HighlightAnim);

		SetKeyboardFocus();
	}
}

void UInventorySlot::OnItemButtonUnHovered()
{
	Image_Border->SetBrushColor(BorderUnHovered_Color);
}

void UInventorySlot::OnRightClick()
{
	/*if (SlotItemSpec.Quantity > 0)
	{
		switch (IInventoryWidgetInterface::Execute_GetActiveWidget(GetOwningPlayer()))
		{
		case EInventoryWidgetType::Inventory:
		case EInventoryWidgetType::Equipment:
			InventoryWindow();
			break;
		case EInventoryWidgetType::Vendor:
		case EInventoryWidgetType::Storage:
			if (IInventoryWidgetInterface::Execute_GetActiveTab(GetOwningPlayer()) == EInventoryWidgetType::Equipment)
			{
				InventoryWindow();
			}
			else
			{
				StoreWindow();
			}
			break;
		default:
			break;
		}
	}*/
}

void UInventorySlot::InventoryWindow()
{
	/*if (SlotItemSpec.EquipmentSlot != EItemSlot::None)
	{
		FText FailureMessage;

		// Inventory -> EquipmentSlot으로 이동 (어느 슬롯으로 이동할지는 아직 모름)
		OwnerEquipment->TransferItemFromInventory(ItemData,
												  FItemData(),
												  OwnerInventory,
												  EInventoryInputMode::Click,
												  FailureMessage);
	}
	else
	{
		// 장착할 아이템이 아니면 사용시도
		TryToUseAnItem();
	}*/
}

void UInventorySlot::TryToUseAnItem()
{}
