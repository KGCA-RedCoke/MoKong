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
	void TrySectionAttack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	UCombatSystem* GetCombatComponent();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	AWeaponBase* GetCurrentWeapon() const;
	/**
	 * 이미 입력이 처리되어 다음 콤보공격이 예약되어 있는지 반환
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	bool IsNextAttackAvailable() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	bool IsAttacking() const;

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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void PreAttack(int32 EffectLevel = 1);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void PostAttack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void SetSectionName(const FName& Name);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	void SetAttacking(const bool Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	AActor* GetTargetActor() const;
};
