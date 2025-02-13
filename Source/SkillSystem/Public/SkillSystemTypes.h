// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "SkillSystemTypes.generated.h"


UENUM(BlueprintType, DisplayName = "스킬 타입")
enum class ESkillType : uint8
{
	Abilities,	// 능력
	Spells		// 신통력
};

UENUM(BlueprintType)
enum class ESkillTreeType : uint8
{
	Foundation,		// 근기
	StaffStances,	// 봉술
	Mysticism,		// 묘술
	Alteration,		// 체술
	Strands,		// 잔털
	Transformations,// 변신
};

USTRUCT(BlueprintType)
struct FSkillSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree", DisplayName= "스킬 타입")
	ESkillType SkillType = ESkillType::Abilities;		// 능력 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree", DisplayName= "필요 포인트")
	int32 SkillPointsRequired = 1;						// 필요한 스킬 포인트
};

USTRUCT(BlueprintType)
struct FSkillDatabase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree")
	UDataTable* SkillTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree")
	FName SkillID;
};

USTRUCT(BlueprintType)
struct FSkillToUnlock
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree")
	ESkillTreeType SkillTreeType = ESkillTreeType::Foundation;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree")
	// bool 
};

USTRUCT(BlueprintType)
struct FSkillTreeWidgetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree", DisplayName= "스킬 이름")
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree", DisplayName= "스킬 설명")
	FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree", DisplayName= "아이콘")
	UTexture2D* Icon;

};

USTRUCT(BlueprintType)
struct FSkillNotification
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree")
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree")
	FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree")
	FLinearColor Color = FLinearColor::White;
};

USTRUCT(BlueprintType)
struct FSkillData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTree")
	float DamageMultiplier = 1.f;
};
