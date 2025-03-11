// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "CombatSystem/AttackSystemTypes.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class UMKAbilitySystemComponent;
class UGameplayEffect;
class UWeaponDataAsset;
class UDidItHitActorComponent;

UCLASS()
class ABILITYSYSTEM_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

public:
	UFUNCTION()
	void OnHitActorAdded(FHitResult LastItem);
	UFUNCTION(BlueprintCallable)
	void InitializeWeapon(USkeletalMeshComponent* InOwnerMeshComponent);

	UFUNCTION(BlueprintCallable)
	void AttachWeapon();
	UFUNCTION(BlueprintCallable)
	void DettachWeapon();
	void PreAttack(TSubclassOf<UGameplayEffect> Effect, float Level);
	void PostAttack();

public:
	/**
	 * 무기 장착 애니메이션을 재생
	 * @param Index 몽타주 인덱스
	 */
	UFUNCTION(BlueprintCallable)
	void SheathWeapon(int32 Index = 0);

	/**
	 * 무기 해제 애니메이션을 재생
	 * @param Index 몽타주 인덱스      
	 */
	UFUNCTION(BlueprintCallable)
	void UnSheathWeapon(int32 Index = 0);

	UFUNCTION(BlueprintCallable)
	void SwapWeapon(FName RowName);

	const FWeaponCombatData& GetWeaponCombatData() const;
	const FWeaponMontageSet& GetWeaponMontageSet() const;

	UFUNCTION()
	void OnHandGoesToWeapon(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> DefaultSceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> SM_Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> SK_Weapon;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	USkeletalMeshComponent* AvatarMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UWeaponDataAsset> WeaponDataAsset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	FDataTableRowHandle AttributeDataHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	FWeaponAttributeData AttributeData;

	UPROPERTY(Transient)
	TObjectPtr<UAnimMontage> CachedMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UDidItHitActorComponent> DidItHitActor;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite)
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(BlueprintReadWrite)
	FGameplayEffectContextHandle EffectContext;

	UPROPERTY(BlueprintReadWrite)
	float EffectLevel;

private:
	UPROPERTY(BlueprintReadOnly, meta= (AllowPrivateAccess = true))
	TObjectPtr<UMKAbilitySystemComponent> AbilitySystemComponent;
};
