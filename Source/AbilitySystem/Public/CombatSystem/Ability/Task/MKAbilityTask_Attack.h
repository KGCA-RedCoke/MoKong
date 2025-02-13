// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MKAbilityTask_Attack.generated.h"

class UCombatSystem;

// 멀티캐스트 델리게이트 정의
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCanAttack);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCanSaveAttack);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAlreadyAttacking);

/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API UMKAbilityTask_Attack : public UAbilityTask
{
	GENERATED_BODY()

public:
	// 생성 메서드
	UFUNCTION(BlueprintCallable,
		Category = "Ability|Tasks",
		meta = (DisplayName = "Attack Task", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
	static UMKAbilityTask_Attack* CreateAttackTask(UGameplayAbility* OwningAbility);


	// 실행 핀 연결을 위한 델리게이트 변수
	UPROPERTY(BlueprintAssignable)
	FOnCanAttack OnCanAttack;

	UPROPERTY(BlueprintAssignable)
	FOnCanSaveAttack OnCanSaveAttack;

	UPROPERTY(BlueprintAssignable)
	FOnAlreadyAttacking OnAlreadyAttacking;

protected:
	virtual void Activate() override;

private:
	// 내부적으로 상태를 처리
	void HandleCombatState();

};
