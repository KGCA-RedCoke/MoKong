// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "CrowdEnemyAIController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIDetectTarget, AActor*, Target);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIDetectSound, const FVector&, Location);

/**
 * 기본 몬스터 AI 컨트롤러
 * !! 잡몹 전용입니다 !!
 * Boss는 Perception, 군중 AI를 사용하지 않기 때문에 ABossAIController를 사용하십쇼 ㅎㅎ
 */
UCLASS()
class MOKONG_API ACrowdEnemyAIController : public ADetourCrowdAIController
{
	GENERATED_BODY()

public:
	ACrowdEnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;


	virtual void BeginPlay() override;

protected:
	UFUNCTION(BlueprintCallable)
	void HandleSenseSight(AActor* Target);
	UFUNCTION(BlueprintCallable)
	void HandleSenseHearing(const FVector& Location);

public:
	UPROPERTY(BlueprintAssignable)
	FOnAIDetectTarget OnAIDetectTarget;
	UPROPERTY(BlueprintAssignable)
	FOnAIDetectSound OnAIDetectSound;

protected:
	UPROPERTY(BlueprintReadOnly, Category="Mokong|AI|Blackboard")
	FName Key_ASC = "OwnerASC";
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
