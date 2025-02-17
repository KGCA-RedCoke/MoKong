// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CombatSystem/Interface/CombatInterface.h"
#include "Data/MKAbilitySystemData.h"
#include "GameFramework/Character.h"
#include "MKAbilityCharacter.generated.h"

class UNiagaraComponent;
class UMotionWarpingComponent;
class UMKAbilitySystemComponent;

/** Hit될 때 이 델리게이트를 호출 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterBaseHitReactDelegate, EHitReactDirection, Direction);

/** 죽을 때 이 델리게이트를 호출 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityCharacterDieDelegate);

/**
 * 어빌리티 시스템 컴포넌트를 가지고 있는 캐릭터
 * 추상 클래스로 사용하며, 이 클래스를 상속받아서 구현해야 한다.
 */
UCLASS(Abstract, NotBlueprintable)
class ABILITYSYSTEM_API AMKAbilityCharacter : public ACharacter,
											  public IAbilitySystemInterface,
											  public ICombatInterface
{
	GENERATED_BODY()

public:
	AMKAbilityCharacter();

protected:
	virtual void BeginPlay() override;

public:
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
	virtual void           SetAttacking_Implementation(const bool Value) override;
	virtual void           SetComboWindowOpen_Implementation(const bool Open) override;
	virtual void           SetNextAttackAvailable_Implementation(const bool Available) override;
	virtual void           SetSectionName_Implementation(const FName& Name) override;
	virtual void           TryComboAttack_Implementation() override;
	virtual void           TrySectionAttack_Implementation() override;
	virtual AActor*        GetTargetActor_Implementation() const override;

	virtual void PreAttack_Implementation(int32 EffectLevel) override;
	virtual void PostAttack_Implementation() override;
	//~~ ICombatInterface End

public:
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

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
	UPROPERTY(BlueprintAssignable)
	FOnAbilityCharacterDieDelegate OnAbilityCharacterDie;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem", Meta = (ShowOnlyInnerProperties))
	FAbilitySystemInitializationData AbilitySystemInitializationData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	UMKAbilitySystemComponent* AbilitySystemComponent;	/** PlayerState에서 관리하기도 함 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem|Input")
	UMKAbilitySet* AbilitySet;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	UMotionWarpingComponent* MotionWarpingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modular|Parts")
	TObjectPtr<USkeletalMeshComponent> Helmet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modular|Parts")
	TObjectPtr<USkeletalMeshComponent> Suit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modular|Parts")
	TObjectPtr<USkeletalMeshComponent> Shoes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modular|Parts")
	TObjectPtr<USkeletalMeshComponent> Gloves;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modular|Parts")
	TObjectPtr<USkeletalMeshComponent> Head;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
	TObjectPtr<UNiagaraComponent> DeathVfx;

	UPROPERTY(BlueprintReadWrite,
		EditAnywhere,
		Category = "AttackSystem",
		meta=(AllowprivateAccess = "true", DisplayPriority = "1"))
	UCombatSystem* CombatComponent;
};
