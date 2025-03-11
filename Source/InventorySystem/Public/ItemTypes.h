// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemTypes.generated.h"


/* 인벤토리 패널 */
UENUM(BlueprintType)
enum class EInventoryPanel : uint8
{
	Weapon,
	Helmet,
	Gloves,
	Suit,
	Shoes,
	Accessory,
	Consumable,
	Items
};

/* 인벤토리 슬롯 (장착 아이템 한정) */
UENUM(BlueprintType)
enum class EItemSlot : uint8
{
	Weapon UMETA(DisplayName = "무기"),
	Gourd UMETA(DisplayName = "담금주"),
	HonBaek UMETA(DisplayName = "혼백"),
	Helmet UMETA(DisplayName = "투구"),
	Gloves UMETA(DisplayName = "장갑"),
	Suit UMETA(DisplayName = "갑옷"),
	Shoes UMETA(DisplayName = "신발"),
	Accessory1 UMETA(DisplayName = "장신구1"),
	Accessory2 UMETA(DisplayName = "장신구2"),
	Count UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EItemUseType : uint8
{
	None,
	Equipment,
	Currency,
	CraftingRecipe
};

/* 인벤토리 패널 정렬방식 */
UENUM(BlueprintType)
enum class ESortMethod : uint8
{
	SimpleSort,
	By_Type,
	By_Rarity,
	By_Value,
	By_Weight
};

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	None,
	Inventory,
	Equipment,
	Crafting,
	Vendor,
	Storage
};

UENUM(BlueprintType)
enum class EInventoryPopupWidgetType : uint8
{
	None,
	SplitStackPopup,
	ConfirmationPopup,
	TextDocumentPopup
};

UENUM(BlueprintType)
enum class EInventoryInputMode : uint8
{
	Click,
	DragDrop
};

UENUM(BlueprintType)
enum class EItemDestination : uint8
{
	None,
	FromInventory,
	FromEquipment,
	FromStore,
	FromStorage,
	FromDropBar
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Common UMETA(DisplayName = "일반급"),
	Uncommon UMETA(DisplayName = "상급"),
	Rare UMETA(DisplayName = "우수급"),
	Epic UMETA(DisplayName = "특급"),
	Mythical UMETA(DisplayName = "신화급"),
	Legendary UMETA(DisplayName = "신선급"),
};


/** Mokong 캐릭터에 부착된 메시 종류 */
UENUM(BlueprintType)
enum class EMeshParts : uint8
{
	Helmet UMETA(DisplayName = "투구"),
	Suit UMETA(DisplayName = "갑옷"),
	Shoes UMETA(DisplayName = "신발"),
	Gloves UMETA(DisplayName = "장갑"),
	Head UMETA(DisplayName = "머리")
};

/* ============================== Structs ===================================*/

/**
 * 아이템 상세 정보
 */
USTRUCT(BlueprintType)
struct FMKItemSpec : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** 표시 이름 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UTexture2D> IconThumbnail;

	/** 아이템 설명 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemCategory Category;

	/** 희귀도 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemRarity Rarity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Index;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSoftClassPtr<class UGameplayEffect>> AssociatedGE;

	friend bool operator==(const FMKItemSpec& Lhs, const FMKItemSpec& Rhs)
	{
		return Lhs.ItemName.IsEqual(Rhs.ItemName, ENameCase::IgnoreCase);
	}
};

USTRUCT(BlueprintType)
struct FMKEquipmentItemSpec : public FMKItemSpec
{
	GENERATED_BODY()

public:
	/** 장착 슬롯 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemSlot EquipmentSlot = EItemSlot::Weapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<class USkeletalMesh> SK_Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<class UStaticMesh> SM_Mesh;
};

USTRUCT(BlueprintType)
struct FMKGourdItemSpec : public FMKEquipmentItemSpec
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Remain;	// 잔량

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HealAmount;	// 회복량
};

USTRUCT(BlueprintType)
struct FSingleDTItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle DT_Row;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity = 1;
};

USTRUCT(BlueprintType)
struct FRandomizedLootTable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* DataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinLootItems = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxLootItems = 0;
};
