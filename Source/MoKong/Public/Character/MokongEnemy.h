// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Actor/MKAbilityNPC.h"
#include "AI/Interface/EnemyAIInterface.h"
#include "MokongEnemy.generated.h"

class UBehaviorTree;
class UDidItHitActorComponent;
class UWidgetComponent;

UCLASS()
class MOKONG_API AMokongEnemy : public AMKAbilityNPC,
								public IEnemyAIInterface,
								public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMokongEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//~~ IEnemyAIInterface ~~//
	virtual float SetMovementSpeed_Implementation(EAIMovementSpeed NewSpeed) override;
	virtual void  GetIdealRange_Implementation(float& OutAttackRange, float& OutDetectRange) override;
	virtual void  OnPlayerSeen_Implementation(AActor* Target) override;
	virtual void  OnHeardSomething_Implementation(const FVector& Location) override;
	//~~ IEnemyAIInterface End ~~//

	//~~ ICombatInterface ~~//
	virtual void PreAttack_Implementation(TSubclassOf<class UGameplayEffect> Effect, float Level = 1) override;
	virtual void PostAttack_Implementation() override;
	virtual void PlayHitReact_Implementation(const FVector&               ImpactLocation, float Damage,
											 const FGameplayTagContainer& AdditionalTags) override;
	//~~ ICombatInterface End ~~//

	//~~ IGenericTeamAgentInterface Begin ~~//
	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(static_cast<uint8>(TeamID)); }
	//~~ IGenericTeamAgentInterface End ~~//

public:
	UFUNCTION(BlueprintCallable)
	void ShowLockOnWidget(bool bShow);

	UFUNCTION(BlueprintCallable)
	void ShowEnemyHPWidget(bool bShow);

	UFUNCTION(BlueprintCallable)
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree.Get(); }

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy|AI")
	void OnHitPlayer(FHitResult LastItem);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy|AI")
	void OnDeath();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Enemy|LockOn")
	TObjectPtr<UWidgetComponent> LockOnWidgetComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "Enemy|Damage")
	TObjectPtr<UWidgetComponent> EnemyHPWidgetComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enemy|Setup")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Enemy|Setup",
		DisplayName = "팀",
		meta = (AllowprivateAccess = "true"))
	ETeamType TeamID;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy|AI")
	AActor* TargetActor;
	UPROPERTY(BlueprintReadWrite, Category = "Enemy|AI")
	EAIEnemyState CurrentState;

	/** 스켈레톤에 부착된 메시 오브젝트에 한정 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy|Trace")
	TObjectPtr<UDidItHitActorComponent> PrimaryAttackTrace;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy|Damage")
	FGameplayEffectSpecHandle DamageSpec;
	UPROPERTY(BlueprintReadWrite, Category = "Enemy|Damage")
	TObjectPtr<UAnimMontage> HitReactMontages_Front;
	UPROPERTY(BlueprintReadWrite, Category = "Enemy|Damage")
	TObjectPtr<UAnimMontage> HitReactMontages_Back;
	UPROPERTY(BlueprintReadWrite, Category = "Enemy|Damage")
	TObjectPtr<UAnimMontage> HitReactMontages_Left;
	UPROPERTY(BlueprintReadWrite, Category = "Enemy|Damage")
	TObjectPtr<UAnimMontage> HitReactMontages_Right;
};
