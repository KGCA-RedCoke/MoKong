// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MoKongTypes.generated.h"

enum class EItemRarity : uint8;

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Stable UMETA(DisplayName = "안정"),
	Battle UMETA(DisplayName = "전투"),
	Action UMETA(DisplayName = "행동"),
	Dead UMETA(DisplayName = "죽음")
};

UENUM(BlueprintType)
enum class EPlayerTransformTypes : uint8
{
	Self UMETA(DisplayName = "오공"),
	HonBaek UMETA(DisplayName = "혼백"),
	Animal UMETA(DisplayName = "동물"),
	Transform UMETA(DisplayName = "변신"),
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

/**
 * 아이템 상세 정보
 */
USTRUCT(BlueprintType)
struct FMKItemSpec
{
	GENERATED_BODY()

	/** 표시 이름 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ItemName;

	/** 희귀도 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EItemRarity Rarity;

	/** 아이템 설명 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<USkeletalMesh> SK_Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UStaticMesh> SM_Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftClassPtr<class UGameplayEffect> AssociatedGE;

};
