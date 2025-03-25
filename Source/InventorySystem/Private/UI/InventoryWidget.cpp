// CopyRight KGCA - Team RedCoke


#include "UI/InventoryWidget.h"

#include "CommonTextBlock.h"
#include "Component/MKEquipmentInventory.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/WidgetSwitcher.h"
#include "Sound/SoundCue.h"
#include "UI/Panel/InventoryPanelButton.h"
#include "UI/Slot/InventorySlot.h"

void UInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	DisplaySample(SampleSlotsToDisplay);
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// FadeIn 애니메이션 재생
	PlayAnimation(FadeIn.Get());

	OwnerInventory = Cast<
		UInventorySystemComponent>(GetOwningPlayer()->GetComponentByClass(UInventorySystemComponent::StaticClass()));
	check(OwnerInventory)


	// TODO: Set Receiver Inventory (상호작용 가능한 개체가 인벤토리를 가지고있을 때( Chest, Vendor)
	// SetReceiverInventory

	// 이벤트 바인딩
	OwnerInventory->OnRefreshInventory.AddUniqueDynamic(this, &ThisClass::RefreshInventory);
	OwnerInventory->OnHighlightInventorySlot.AddUniqueDynamic(this, &ThisClass::HighlightSlot);
	OwnerInventory->OnSwitchedActivePanel.AddUniqueDynamic(this, &ThisClass::SwitchInventoryPanel);
	/*SortWindow->OnEndSorting.AddUniqueDynamic(this, &ThisClass::OnSortingFinished);*/

	for (auto& Panel : OwnerInventory->InventoryPanels)
	{
		CreateInventoryPanel(Panel.Key);
	}

	SetActivePanel();
}

void UInventoryWidget::CreateInventoryPanel(const EInventoryPanel PanelToCreate)
{
	if (auto* Array = OwnerInventory->GetInventoryArray(PanelToCreate))
	{
		SetInventorySlots(*Array, Array->Num(), GetUniformGridFromPanel(PanelToCreate));
	}
}

void UInventoryWidget::SetInventorySlots(const TArray<FMKInventoryItemSpec>& Inventory, int32 InventorySize,
										 UUniformGridPanel*                  UniformGridPanel)
{
	int32 Local_Row    = 0;
	int32 Local_Column = 0;

	// 그리드패널 초기화
	UniformGridPanel->ClearChildren();

	for (int i = 0; i < InventorySize; ++i)
	{
		// 인벤토리 크기만큼 슬롯을 생성
		UInventorySlot* InventorySlot = CreateWidget<UInventorySlot>(
																	 GetOwningPlayer(),
																	 InventorySlotClass);

		// 슬롯 유효성 확인
		if (IsValid(InventorySlot))
		{
			// 슬롯위젯에 데이터 넣기
			InventorySlot->InitializeWidget(OwnerInventory, this, i);

			// 슬롯위젯에 아이템 데이터 넣기
			InventorySlot->OverwriteSlot(Inventory[i]);

			// 유니폼그리드에 넣기
			if (UUniformGridSlot* GridSlot = UniformGridPanel->AddChildToUniformGrid(
				 InventorySlot,
				 Local_Row,
				 Local_Column))
			{
				GridSlot->SetHorizontalAlignment(HAlign_Fill);
				GridSlot->SetVerticalAlignment(VAlign_Fill);
			}
		}

		// 오른쪽부터 순서대로 채워주고
		Local_Column++;

		// 오른쪽끝에 도달하면 다음 줄로 넘어감
		if (Local_Column >= RowLength)
		{
			Local_Column = 0;
			Local_Row++;
		}
	}
}

void UInventoryWidget::DisplaySample(int32 NumToDisplay)
{
	/*UUniformGridPanel* UniformPanel = GetUniformGridFromPanel(EInventoryPanel::Weapon);
	UniformPanel->ClearChildren();

	int32 Local_Row    = 0;
	int32 Local_Column = 0;

	for (int i = 0; i < NumToDisplay; ++i)
	{
		if (InventorySlotClass)
		{
			// 인벤토리 슬롯 생성
			if (UInventorySlot* InventorySlot = CreateWidget<UInventorySlot>(this, InventorySlotClass))
			{
				// 균등한 패널에 슬롯을 추가
				if (UUniformGridSlot* GridSlot = UniformPanel->AddChildToUniformGrid(
					 InventorySlot,
					 Local_Row,
					 Local_Column))
				{
					GridSlot->SetHorizontalAlignment(HAlign_Fill);
					GridSlot->SetVerticalAlignment(VAlign_Fill);
				}
			}
		}

		// 오른쪽부터 순서대로 채워주고
		Local_Column++;

		// 오른쪽끝에 도달하면 다음 줄로 넘어감
		if (Local_Column >= RowLength)
		{
			Local_Column = 0;
			Local_Row++;
		}
	}*/
}

EInventoryPanel UInventoryWidget::GetActivePanel() const
{
	return ActivePanel;
}

TObjectPtr<UUniformGridPanel> UInventoryWidget::GetUniformGridFromPanel(const EInventoryPanel Panel) const
{
	return InventoryPanels.FindRef(Panel);
}

void UInventoryWidget::SetActivePanel()
{
	for (auto& Panel : PanelButtons)
	{
		Panel.Value->OnClickedButtonPanel.AddUniqueDynamic(this, &ThisClass::SwitchInventoryPanel);
		Panel.Value->RefreshPanelButton(ActivePanel);

	}

	if (UUniformGridPanel* GridPanel = GetUniformGridFromPanel(ActivePanel))
	{
		Panel_WidgetSwitcher->SetActiveWidget(GridPanel);

		OwnerInventory->SwitchActivePanel(ActivePanel);

		SetPanelTitle(ActivePanel);
	}

}

void UInventoryWidget::SetPanelTitle(EInventoryPanel Panel) const
{
	const FText NameText = PanelButtons.FindRef(Panel)->GetPanelName();

	PanelName->SetText(NameText);
}

void UInventoryWidget::SetFocusToSlot(int32 SlotIndex)
{
	const UUniformGridPanel* UniformGridPanel = GetUniformGridFromPanel(GetActivePanel());
	if (IsValid(UniformGridPanel))
	{
		UWidget* ChildWidget = UniformGridPanel->GetChildAt(SlotIndex);
		if (IsValid(ChildWidget))
		{
			FocusedSlot = SlotIndex;

			ChildWidget->SetKeyboardFocus();
		}
	}
}

void UInventoryWidget::ResetSlotFocus()
{
	SetFocusToSlot(FocusedSlot);
}

void UInventoryWidget::RefreshInventory(const EInventoryPanel Panel)
{
	CreateInventoryPanel(Panel);
}

void UInventoryWidget::HighlightSlot(const int SlotIndex)
{
	const UUniformGridPanel* UniformGridPanel = GetUniformGridFromPanel(GetActivePanel());
	if (IsValid(UniformGridPanel))
	{
		UInventorySlot* ChildWidget = Cast<UInventorySlot>(UniformGridPanel->GetChildAt(SlotIndex));
		if (IsValid(ChildWidget))
		{
			ChildWidget->HighlightSlot();
		}
	}
}

void UInventoryWidget::SwitchInventoryPanel(const EInventoryPanel NewPanel)
{
	// 변경하려는 패널과 기존 패널이 달라야함
	if (GetActivePanel() != NewPanel)
	{
		// 스위치 소리 재생
		PlaySound(SwitchSoundCue);

		// 패널을 바꿔줌
		Panel_WidgetSwitcher->SetActiveWidget(GetUniformGridFromPanel(NewPanel));

		//
		OwnerInventory->SwitchActivePanel(NewPanel);

		for (auto& Panel : PanelButtons)
		{
			Panel.Value->RefreshPanelButton(NewPanel);
		}

		// 패널의 제목(카테고리) 업데이트
		SetPanelTitle(NewPanel);

		// 패널로 포커스 이동
		SetFocusToSlot(0);
	}
}

void UInventoryWidget::OnSortingFinished() {}
