// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/AttackSystemTypes.h"
#include "CombatSystem/Interface/CombatInterface.h"
#include "Components/ActorComponent.h"
#include "CombatSystemComp.generated.h"

struct FGameplayTagContainer;
class UMKAbilitySystemComponent;
class UAbilitySystemComponent;
class AWeaponBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ABILITYSYSTEM_API UCombatSystem : public UActorComponent, public ICombatInterface
{
	GENERATED_BODY()

public:
	UCombatSystem();

public:
	UFUNCTION(BlueprintCallable)
	void InitializeCombatSystem(USkeletalMeshComponent* InAvatarMeshComponent);

	// ICombatInterface
	virtual void TryComboAttack_Implementation() override;
	virtual void TrySectionAttack_Implementation(FName SectionName) override;

	virtual void PreAttack_Implementation(TSubclassOf<UGameplayEffect> Effect, float Level) override;
	virtual void PostAttack_Implementation() override;

	virtual UCombatSystem* GetCombatComponent_Implementation() override;
	virtual FName          GetSectionName_Implementation() const override;
	virtual bool           IsNextAttackAvailable_Implementation() const override;
	virtual bool           IsComboWindowOpen_Implementation() const override;
	virtual void           SetAttacking_Implementation(const bool Value) override;
	virtual void           SetNextAttackAvailable_Implementation(const bool Available) override;
	virtual void           SetComboWindowOpen_Implementation(const bool Open) override;
	virtual void           SetSectionName_Implementation(const FName& Name) override;
	virtual AActor*        GetTargetActor_Implementation() const override;

	virtual void PlayHitReact_Implementation(const FVector& ImpactLocation, float Damage,
											 const FGameplayTagContainer& AdditionalTags) override;
	// ~ICombatInterface

	UFUNCTION(BlueprintCallable)
	bool CheckCommit();

	void SetTargetActor(AActor* InTargetActor);

	void ResetComboData();

	void UpdateComboData(TObjectPtr<UAnimMontage>& MontageToPlay, FName& SectionToPlay, bool bInAir = false);

	UFUNCTION(BlueprintCallable)
	void SwapWeapon(EWeaponType Type, USkeletalMeshComponent* InMeshComponent = nullptr);

protected:
	/** WeaponBase액터를 이용해서 무기를 부착할거라면 true 메시자체에 무기가 있는경우 false */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|Startup")
	bool bUseWeaponActor;

	/** 부착할 소켓 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|Startup", meta=(EditCondition="!bUseWeaponActor"))
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|Startup", meta=(EditCondition="!bUseWeaponActor"))
	FDataTableRowHandle AttributeDataHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|Startup", meta = (EditCondition="bUseWeaponActor"))
	TMap<EWeaponType, TSubclassOf<AWeaponBase>> WeaponClasses;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|State")
	TMap<EWeaponType, AWeaponBase*> SpawnedWeapons;

	UPROPERTY()
	USkeletalMeshComponent* AvatarMeshComponent;

	UPROPERTY()
	TObjectPtr<UMKAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|State")
	EWeaponType CurrentWeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|State")
	TObjectPtr<AWeaponBase> CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat|State")
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Combat|State")
	FName CurrentWeaponRowName;	// 현재 장착중인 무기의 테이블에서의 행 이름

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Combat|State")
	int32 ComboIndex;			// 현재 콤보 인덱스

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Combat|State")
	uint8 bComboWindowOpened : 1;		// 콤보 윈도우가 열려있는지

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Combat|State")
	uint8 bAttacking : 1;		// 현재 콤보 애니메이션을 재생중인지

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Combat|State")
	uint8 bReadyToNextAttack : 1;	// 다음 콤보가 예약 되었는지

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Combat|State")
	FName CurrentSectionName = "1";	// 현재 콤보 섹션 이름
};
