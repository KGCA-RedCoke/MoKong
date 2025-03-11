// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class AWeaponBase;
class UCombatSystem;

// This class does not need to be modified.
UINTERFACE()
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ABILITYSYSTEM_API ICombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void TryComboAttack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void TrySectionAttack(FName SectionName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	UCombatSystem* GetCombatComponent();

	/**
	 * 이미 입력이 처리되어 다음 콤보공격이 예약되어 있는지 반환
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	bool IsNextAttackAvailable() const;

	/**
	 * 다음 콤보 입력이 가능한 구간인지 반환
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	bool IsComboWindowOpen() const;

	/**
	 * 콤보공격 입력을 예약
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void SetNextAttackAvailable(const bool Available);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void SetComboWindowOpen(const bool Open);

	/**
	 * 콤보의 섹션 이름을 반환
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	FName GetSectionName() const;

	/**
	 * 공격하기 전에 사전 정보 처리가 필요하면 여기서 재정의하세요
	 * @param GameplayEffect
	 * @param EffectLevel (GameplayEffect Level)
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void PreAttack(TSubclassOf<class UGameplayEffect> Effect, float Level = 1);

	/**
	 * 공격이 끝나고 처리할 것이 있으면 여기서 재정의하세요
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void PostAttack();

	/**
	 * 수동으로 콤보공격의 섹션 이름을 설정
	 * @param Name 몽타주 섹션 이름
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void SetSectionName(const FName& Name);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void SetAttacking(const bool Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	AActor* GetTargetActor() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void PlayHitReact(const FVector& ImpactLocation, float Damage, const FGameplayTagContainer& AdditionalTags);
};
