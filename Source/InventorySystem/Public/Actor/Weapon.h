// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "ItemBase.h"
#include "Data/DataAsset_Weapon.h"
#include "Weapon.generated.h"

class UGameplayEffect;
class UWeaponDataAsset;
class UDidItHitActorComponent;

UCLASS()
class INVENTORYSYSTEM_API AWeapon : public AItemBase
{
	GENERATED_BODY()

public:
	AWeapon();

protected:
	virtual void BeginPlay() override;

	virtual void InitializeData() override;

public:
	UFUNCTION(BlueprintCallable)
	void InitializeWeapon(USkeletalMeshComponent* InOwnerMeshComponent);

	UFUNCTION(BlueprintCallable)
	void SwapWeapon(FName RowName);

	UFUNCTION(BlueprintCallable)
	void AttachWeapon();
	UFUNCTION(BlueprintCallable)
	void DettachWeapon();

	void PreAttack(TSubclassOf<UGameplayEffect> Effect, float Level);
	void PostAttack();

	const FWeaponCombatData& GetWeaponCombatData() const;
	const FWeaponMontageSet& GetWeaponMontageSet() const;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadWrite)
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY(BlueprintReadWrite)
	FGameplayEffectContextHandle EffectContext;

	UPROPERTY(BlueprintReadWrite)
	float EffectLevel;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> DefaultSceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> SM_Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> SK_Weapon;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UDidItHitActorComponent> DidItHitActor;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Data")
	TObjectPtr<USkeletalMeshComponent> AvatarMeshComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UDataAsset_Weapon> WeaponDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	FMKEquipmentItemSpec WeaponData;
};
