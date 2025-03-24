// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "ATPCCameraLockOnTargetObject.h"
#include "GenericTeamAgentInterface.h"
#include "MoKongTypes.h"
#include "AbilitySystem/Public/Actor/MKAbilityPlayer.h"
#include "AI/Interface/EnemyAIInterface.h"
#include "Interface/ModularCharacter.h"
#include "MKPlayer.generated.h"

class UInventorySystemComponent;
class AMKPlayerController;
class AGourd;
class UTransformData;
class UMeshPartsData;
class AMokongEnemy;
class UATPCCameraComponent;
class UMotionWarpingComponent;
class UParkourComponent;
class ULocomotionComponent;
class UFootStepSFXComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, EPlayerState, NewState);

UCLASS(Blueprintable)
class MOKONG_API AMKPlayer : public AMKAbilityPlayer, public IGenericTeamAgentInterface, public IModularCharacter
{
	GENERATED_BODY()

public:
	AMKPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:
	UFUNCTION(BlueprintCallable)
	AMokongEnemy* GetNearestEnemy(float Distance = 1000.f);
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UInventorySystemComponent* GetInventorySystemComponent() const { return InventorySystemComponent.Get(); }

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
	virtual void    PreAttack_Implementation(TSubclassOf<UGameplayEffect> Effect, float Level) override;
	virtual AActor* GetTargetActor_Implementation() const override;
	//~~ CombatInterface End ~~//

	//~~ ModularCharacter Begin ~~//
	virtual void EquipItem_Implementation(const FMKInventoryItemSpec& ItemData) override;
	//~~ ModularCharacter End ~~//

	UFUNCTION(BlueprintCallable)
	bool IsLockingOn() const;

	UFUNCTION(BlueprintCallable)
	void Transform();

	UFUNCTION(BlueprintCallable)
	void SetMeshParts(EMeshParts MeshPart, USkeletalMesh* NewMesh);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowprivateAccess = "true"), DisplayName= "혼백")
	TObjectPtr<USkeletalMeshComponent> HonBaekMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowprivateAccess = "true"), DisplayName= "호리병")
	TObjectPtr<AGourd> GourdActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowprivateAccess = "true"), DisplayName = "카메라")
	TObjectPtr<UATPCCameraComponent> PlayerCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowprivateAccess = "true"))
	TObjectPtr<USceneCaptureComponent2D> SceneCaptureComponent;

	UPROPERTY(EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Player|Setup",
		DisplayName = "팀",
		meta = (AllowprivateAccess = "true"))
	ETeamType TeamID;

	UPROPERTY(BlueprintReadWrite, Category = "Player|Target", meta=(AllowprivateAccess = "true"))
	TObjectPtr<AMokongEnemy> TargetEnemy;

	/** 저장된 파츠 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowprivateAccess = "true"), Category="Player|Transform")
	TObjectPtr<UMeshPartsData> CachedMeshPartsData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowprivateAccess = "true"), Category="Player|Transform")
	TObjectPtr<UTransformData> CachedTransformData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowprivateAccess = "true"), Category="Player|Transform")
	TSoftClassPtr<AGourd> GourdClass;

	UPROPERTY(BlueprintReadWrite, meta=(AllowprivateAccess = "true"))
	bool bCanInteract;

	UPROPERTY(BlueprintReadWrite, meta=(AllowprivateAccess = "true"), Category= "Player|Controller")
	TObjectPtr<UInventorySystemComponent> InventorySystemComponent;
};
