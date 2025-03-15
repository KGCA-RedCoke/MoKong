// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.h"
#include "Components/ActorComponent.h"
#include "InventorySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRefreshInventory, const EInventoryPanel, Panel);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemovedFromInventoryArray, const FMKInventoryItemSpec&, ItemData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddedToInventoryArray, const FMKInventoryItemSpec&, ItemData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHighlightInventorySlot, const int, SlotIndex);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSwitchedActivePanel, const EInventoryPanel, Panel);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UInventorySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventorySystemComponent();

protected:
	virtual void BeginPlay() override;

public:
	/**
	 * 인벤토리 초기화 진행
	 * @param PlayerController 이 인벤토리를 소유할 컨트롤러
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void InitializeInventory(APlayerController* PlayerController);

	/**
	 * 패널 정보에 따라 패널을 초기화
	 * @param Panel 초기화할 패널 
	 */
	void InitializePanel(EInventoryPanel Panel);

	/**
	 * 인벤토리 배열에 아이템을 추가 (배열에만 추가되고 다른 처리는 되지않음)
	 * @param ItemSpec 추가될 아이템
	 * @param SlotIndex 배열의 특정 슬롯에 넣어야한다면 인덱스번호
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItemToInventoryArray(const FMKItemSpec& ItemSpec, int SlotIndex);

	/**
	 * 인벤토리의 아이템의 특정 슬롯에 수량 추가(Stackable한 아이템이면) 
	 * @param ItemData 추가될 아이템 데이터
	 * @param SlotIndex 슬롯 인덱스
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddToStackInInventory(const FMKItemSpec& ItemData, int32 SlotIndex);

	/**
	 * 인벤토리 특정 패널 슬롯의 아이템을 삭제
	 * @param Panel 선택 패널
	 * @param SlotIndex 패널 슬롯 인덱스
	 * @param bRemoveAll 모두 삭제할지 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemFromPanel(const EInventoryPanel Panel, const int SlotIndex, bool bRemoveAll = false);

	/**
	 * 인벤토리 배열에서 아이템을 삭제 (배열에만 삭제되고 다른 처리는 되지않음) 
	 * @param InItemData 삭제할 아이템 데이터
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemFromInventoryArray(const FMKInventoryItemSpec& InItemData);

	/**
	 * 활성화된 패널을 새로운 패널로 바꿔준다. (OnSwitchedActivePanel 호출)
	 * @param InventoryPanel 바꿔줄 패널
	 */
	void SwitchActivePanel(const EInventoryPanel InventoryPanel);

	/**
	 * 인벤토리의 특정 패널의 특정 슬롯의 아이템 반환
	 * @param Panel 확인할 패널
	 * @param SlotIndex 확인할 인덱스
	 * @return 아이템 데이터
	 */
	FMKInventoryItemSpec* GetItemBySlot(const EInventoryPanel Panel, const int SlotIndex);

	TArray<FMKInventoryItemSpec>* GetInventoryArray(const EInventoryPanel Panel);

	EInventoryPanel GetActivePanel() const;

public:
#pragma region Sorting Function
	void        SortInventory(ESortMethod SortMethod, EInventoryPanel SinglePanel, bool bSortAllPanel);
	static void SortSimple(TArray<FMKInventoryItemSpec>& Inventory, TArray<FMKInventoryItemSpec>& SortedInventory);
	/*static void SortByType(TArray<FMKInventoryItemSpec>& Inventory);*/
	static void SortByRarity(TArray<FMKInventoryItemSpec>& Inventory);
#pragma endregion

public:
#pragma region Delegate
	/* 인벤토리가 업데이트될 때 호출 */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnRefreshInventory OnRefreshInventory;
	/* 인벤토리에서 아이템을 제거할 때 호출 */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnRemovedFromInventoryArray OnRemovedFromInventories;
	/* 인벤토리에 아이템이 추가될 때 호출*/
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnAddedToInventoryArray OnAddedToInventories;
	/* 인벤토리 슬롯에 커서가 올라갈 때나 게임패드의 포커스가 있을 때 호출 */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnHighlightInventorySlot OnHighlightInventorySlot;
	/* 인벤토리의 패널카테고리가 바뀔 때 호출 */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnSwitchedActivePanel OnSwitchedActivePanel;
#pragma endregion

protected:
	/* true일 경우 DefaultSortingMethod에 따른 아이템 정렬 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Setting")
	bool bSortInitialItems = true;

	/* 기본 정렬 방식 지정 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Setting")
	ESortMethod DefaultSortingMethod = ESortMethod::By_Type;

	/* true일 경우 인벤토리 위젯 닫을 때 최신 상태로 업데이트*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Setting")
	bool bRefreshOnClose = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory|Setting")
	TSet<EInventoryPanel> PanelsToUse;

	// 초기 인벤토리 패널 수
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Setting")
	int32 InitialInventoryPanelCount = 1;

	/** */
	TMap<EInventoryPanel, TArray<FMKInventoryItemSpec>> InventoryPanels;

	/* 현재 활성화된 인벤토리 패널 */
	UPROPERTY(BlueprintReadWrite, Category= "Inventory")
	EInventoryPanel ActivePanel = EInventoryPanel::Items;

	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	TObjectPtr<APlayerController> PlayerControllerRef;
};
