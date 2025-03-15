// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.h"
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
