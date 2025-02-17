// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossEnemyAIController.generated.h"


/**
 * 보스 전용 AI 컨트롤러
 * 타겟 체크를 하지 않는다. (Perception을 사용하지 않음)
 * - Target이 플레이어로 고정되어 있음
 */
UCLASS()
class MOKONG_API ABossEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABossEnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category="Mokong|AI|Blackboard")
	FName Key_Target = "Target";
	UPROPERTY(BlueprintReadOnly, Category="Mokong|AI|Blackboard")
	FName Key_State = "State";
	UPROPERTY(BlueprintReadOnly, Category="Mokong|AI|Blackboard")
	FName Key_CurrentInterest = "PointOfInterest";
	UPROPERTY(BlueprintReadOnly, Category="Mokong|AI|Blackboard")
	FName Key_AttackRadius = "AttackRadius";
	UPROPERTY(BlueprintReadOnly, Category="Mokong|AI|Blackboard")
	FName Key_DefenseRadius = "DefenseRadius";
};
