// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyAIInterface.generated.h"

UENUM(BlueprintType)
enum class EAIMovementSpeed : uint8
{
	Patrol UMETA(DisplayName = "순찰"),
	Chase UMETA(DisplayName = "추격"),
	Attack UMETA(DisplayName = "대치"),
	ETC1 UMETA(DisplayName = "기타"),
	ETC2 UMETA(DisplayName = "기타2"),
	ETC3 UMETA(DisplayName = "기타3"),
	ETC4 UMETA(DisplayName = "기타4"),
	ETC5 UMETA(DisplayName = "기타5"),
};

/** Perception을 사용하는 AI들에게 판별 Type */
UENUM(BlueprintType)
enum class ETeamType : uint8
{
	Player UMETA(DisplayName = "플레이어"),
	Enemy UMETA(DisplayName = "적"),
	Neutral UMETA(DisplayName = "NPC"),
	Friendly UMETA(DisplayName = "아군")
};

/** 어지간하면 GameplayTag로 상태를 관리하시기 바랍니다. (그러면 Ability랑 Tag가 연동되어 더 자연스러운 추적 가능) */
UENUM(BlueprintType, meta=(ToolTip="적 AI 상태"))
enum class EAIEnemyState : uint8
{
	Idle UMETA(DisplayName = "대기"),
	Patrol UMETA(DisplayName = "순찰"),
	Investigate UMETA(DisplayName = "조사"),
	Chase UMETA(DisplayName = "추격"),
	Attack UMETA(DisplayName = "공격"),
	Dead UMETA(DisplayName = "죽음"),
	Frozen UMETA(DisplayName = "경직"),
	ETC1 UMETA(DisplayName = "기타"),
	ETC2 UMETA(DisplayName = "기타2"),
	ETC3 UMETA(DisplayName = "기타3"),
	ETC4 UMETA(DisplayName = "기타4"),
	ETC5 UMETA(DisplayName = "기타5"),
};

// This class does not need to be modified.
UINTERFACE()
class UEnemyAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MOKONG_API IEnemyAIInterface
{
	GENERATED_BODY()

public:
	/* 이동속도 수동 지정 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mokong|AI")
	float SetMovementSpeed(EAIMovementSpeed NewSpeed);

	/** 범위 설정 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mokong|AI")
	void GetIdealRange(float& OutAttackRange, float& OutDetectRange);

	/** 플레이어 발견시 재정의 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mokong|AI")
	void OnPlayerSeen(AActor* Target);

	/** 사운드 감지시 재정의 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mokong|AI")
	void OnHeardSomething(const FVector& Location);

	/** 플레이어가 JXSQ시전 시 발동 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mokong|AI")
	void OnPlayerJXSQ(AActor* NewTarget);

	/** 플레이어 JXSQ시전 만료 시 발동 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mokong|AI")
	void OnPlayerJXSQExpired();
};
