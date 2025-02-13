// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CombatSystem/Interface/CombatInterface.h"
#include "Data/MKAbilitySystemData.h"
#include "GameFramework/Character.h"
#include "MKAbilityCharacter.generated.h"

class UMotionWarpingComponent;
class UMKAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterBaseHitReactDelegate, EHitReactDirection, Direction);


/**
 * 어빌리티 시스템 컴포넌트를 가지고 있는 캐릭터
 * 추상 클래스로 사용하며, 이 클래스를 상속받아서 구현해야 한다.
 */
UCLASS(Abstract, NotBlueprintable)
class ABILITYSYSTEM_API AMKAbilityCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AMKAbilityCharacter();

protected:
	virtual void BeginPlay() override;

public:
	USkeletalMeshComponent* GetAvatarMesh() const { return AvatarMesh.Get(); }

	UFUNCTION(BlueprintImplementableEvent)
	void PostInitializeAbilitySystem();

	UFUNCTION(BlueprintPure)
	const FAbilitySystemInitializationData& GetAbilitySystemInitializationData() const
	{
		return AbilitySystemInitializationData;
	}

public:
	//~~ IAbilitySystemInterface Begin
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UFUNCTION(BlueprintPure, Category="AbilitySystem")
	UMKAbilitySystemComponent* GetMKAbilitySystemComponent() const;
	//~~ IAbilitySystemInterface End

	//~~ ICombatInterface Begin
	virtual UCombatSystem* GetCombatComponent_Implementation() override;
	virtual FName          GetSectionName_Implementation() const override;
	virtual bool           IsComboWindowOpen_Implementation() const override;
	virtual bool           IsNextAttackAvailable_Implementation() const override;
	virtual bool           IsAttacking_Implementation() const override;
	virtual void           SetAttacking_Implementation(const bool Value) override;
	virtual void           SetComboWindowOpen_Implementation(const bool Open) override;
	virtual void           SetNextAttackAvailable_Implementation(const bool Available) override;
	virtual void           SetSectionName_Implementation(const FName& Name) override;
	virtual void           TryComboAttack_Implementation() override;
	virtual void           TrySectionAttack_Implementation() override;
	virtual AWeaponBase*   GetCurrentWeapon_Implementation() const override;
	virtual AActor*        GetTargetActor_Implementation() const override;

	virtual void PreAttack_Implementation(int32 EffectLevel) override;
	virtual void PostAttack_Implementation() override;
	//~~ ICombatInterface End

public:
	UFUNCTION(BlueprintNativeEvent)
	void PlayHitReact(EHitReactDirection Direction);

	UFUNCTION(BlueprintCallable)
	void UpdateMotionWarpingTargetLocation(FName WarpName, const FVector& TargetLocation);
	UFUNCTION(BlueprintCallable)
	void UpdateMotionWarpingTargetLocationAndRotation(FName           WarpName, const FVector& TargetLocation,
													  const FRotator& TargetRotation);

public:
	UPROPERTY(BlueprintAssignable)
	FCharacterBaseHitReactDelegate OnCharacterBaseHitReact;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem", Meta = (ShowOnlyInnerProperties))
	FAbilitySystemInitializationData AbilitySystemInitializationData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	UMKAbilitySystemComponent* AbilitySystemComponent;	/** PlayerState에서 관리하기도 함 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem|Input")
	UMKAbilitySet* AbilitySet;

	UPROPERTY(meta=(AllowPrivateAccess = "true"))
	UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modular")
	TObjectPtr<USkeletalMeshComponent> AvatarMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modular|Parts")
	TObjectPtr<USkeletalMeshComponent> Helmet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modular|Parts")
	TObjectPtr<USkeletalMeshComponent> UpperBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modular|Parts")
	TObjectPtr<USkeletalMeshComponent> LowerBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modular|Parts")
	TObjectPtr<USkeletalMeshComponent> Shoes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modular|Parts")
	TObjectPtr<USkeletalMeshComponent> Gloves;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modular|Parts")
	TObjectPtr<USkeletalMeshComponent> Head;

	UPROPERTY(BlueprintReadWrite,
		EditAnywhere,
		Category = "AttackSystem",
		meta=(AllowprivateAccess = "true", DisplayPriority = "1"))
	UCombatSystem* CombatComponent;
};
