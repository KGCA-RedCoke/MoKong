// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "AttackSystemTypes.generated.h"

class AWeaponBase;

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	Idle UMETA(DisplayName = "대기 상태"),
	AttackWindowOpened UMETA(DisplayName = "공격 가능 상태"),
	Attacking UMETA(DisplayName = "공격 중"),
	AttackFinished UMETA(DisplayName = "공격 종료"),
	WeaponHandling UMETA(DisplayName = "무기 장착/해제 중"),
	Etc
};

UENUM(BlueprintType)
enum class ECombatInput: uint8
{
	None,
	LightAttack,
	HeavyAttack,
	Skill1,
	Skill2,
	Skill3,
	Skill4,
	Special,
	EComboInput_MAX
};

UENUM(BlueprintType)
enum class EWeaponType: uint8
{
	Unarmed UMETA(DisplayName = "무장 해제"),
	Sword UMETA(DisplayName = "카타나"),
	Bow UMETA(DisplayName = "활"),
	SwordShield UMETA(DisplayName = "검과 방패"),
	BoStaff UMETA(DisplayName = "봉"),
	EWeaponType_MAX
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

/** 몽타주 집합 셋 */
USTRUCT(BlueprintType)
struct FWeaponMontageSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "장착 몽타주")
	TArray<TObjectPtr<UAnimMontage>> EquipMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "해제 몽타주")
	TArray<TObjectPtr<UAnimMontage>> UnEquipMontage;
};

USTRUCT(BlueprintType)
struct FWeaponAttachmentData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "무기 장착 소켓")
	FName HandSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "무기 부착 소켓")
	FName HolsterSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "칼집 장착 소켓")
	FName ScabbardSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "칼집 부착 소켓")
	FName ScabbardHolsterSocket;
};

USTRUCT(BlueprintType)
struct FWeaponCombatData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, TObjectPtr<UAnimMontage>> GroundComboMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, TObjectPtr<UAnimMontage>> AirComboMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, TObjectPtr<UAnimMontage>> AbilityMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, TObjectPtr<UAnimMontage>> SkillMontages;

};

USTRUCT(BlueprintType)
struct FWeaponAttributeData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "메시")
	TObjectPtr<UStaticMesh> WeaponStaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "메시(스켈레탈)")
	TObjectPtr<USkeletalMesh> WeaponSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "희귀도")
	EItemRarity Rarity = EItemRarity::Common;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "공격")
	int AttackPower = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "치명타율")
	float CriticalRate = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "빙결 저항")
	float FreezeResistance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "연소 저항")
	float FireResistance = 0.f;
};
