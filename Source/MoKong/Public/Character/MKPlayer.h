// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "ATPCCameraLockOnTargetObject.h"
#include "GenericTeamAgentInterface.h"
#include "MoKongTypes.h"
#include "AbilitySystem/Public/Actor/MKAbilityPlayer.h"
#include "AI/Interface/EnemyAIInterface.h"
#include "MKPlayer.generated.h"

class AMokongEnemy;
class UATPCCameraComponent;
class UMotionWarpingComponent;
class UParkourComponent;
class ULocomotionComponent;
class UFootStepSFXComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, EPlayerState, NewState);

UCLASS(Blueprintable)
class MOKONG_API AMKPlayer : public AMKAbilityPlayer, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AMKPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(BlueprintCallable)
	AMokongEnemy* GetNearestEnemy(float Distance = 1000.f);

private:
	UFUNCTION()
	void PlayerStateChaneDelegate(EPlayerState NewState);
	UFUNCTION()
	void OnLockOnTargetChange(AActor* NewTarget, EATPCChangeTargetReason ChangeTargetReason);

public:
	//~~ IGenericTeamAgentInterface Begin ~~//
	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(static_cast<uint8>(TeamID)); }
	//~~ IGenericTeamAgentInterface End ~~//

	//~~ CombatInterface Begin ~~//
	virtual void    PreAttack_Implementation(float EffectLevel = 1) override;
	virtual AActor* GetTargetActor_Implementation() const override;
	//~~ CombatInterface End ~~//

	UFUNCTION(BlueprintCallable)
	bool IsLockingOn() const;

	UFUNCTION(BlueprintCallable)
	void Transform();

private:
	void ChangeActiveMesh(const EPlayerTransformTypes TransformType);

public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateChanged;

private:
	UPROPERTY(BlueprintReadWrite,
		EditAnywhere,
		Category = "FootStep",
		DisplayName="착지 SFX",
		meta=(AllowprivateAccess = "true", DisplayPriority = "1"))
	TObjectPtr<UFootStepSFXComponent> FootstepComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Locomotion|Base", meta=(AllowprivateAccess = "true"))
	TObjectPtr<ULocomotionComponent> LocomotionComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Locomotion|Parkour", meta=(AllowprivateAccess = "true"))
	TObjectPtr<UParkourComponent> ParkourComponent;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Player|State", meta=(AllowprivateAccess = "true"))
	EPlayerState CurrentPlayerState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowprivateAccess = "true"), DisplayName= "호리병")
	TObjectPtr<UStaticMeshComponent> GourdMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"), DisplayName = "카메라")
	TObjectPtr<UATPCCameraComponent> PlayerCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> TransformMeshComponent;	// 변신 메시

	UPROPERTY(EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Player|Setup",
		DisplayName = "팀",
		meta = (AllowprivateAccess = "true"))
	ETeamType TeamID;

	UPROPERTY(BlueprintReadWrite, Category = "Player|Target", meta=(AllowprivateAccess = "true"))
	TObjectPtr<AMokongEnemy> TargetEnemy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bCanInteract = false;
};
