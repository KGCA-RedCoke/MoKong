// copyright Jacob Lim

#pragma once

#include "CoreMinimal.h"
#include "LocomotionComponent_Base.h"
#include "Data/Asset/ParkourAction.h"
#include "ParkourComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class LOCOMOTION_API UParkourComponent : public ULocomotionComponent_Base
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

#pragma region Callback
	/* ACharacter의 PostCharacterMovement에서 호출 (캐릭터의 이동이 업데이트 된 후에 호출) */
	virtual void OnCharacterMovementUpdated(float DeltaSeconds, FVector OldLocation, FVector OldVelocity) override;
	/* 대부분의 파쿠르모션은 몽타주로 진행되어서 빠져 나갈때 자연스럽게 상태 변화를 부여 */
	virtual void OnMontageBlendOut(UAnimMontage* Montage, bool bInterrupted) override;
#pragma endregion

	UFUNCTION(BlueprintCallable)
	void HandleRollAndDodgeAction(bool  bPressed, EDodgeType         DodgeType, EMovementDirection Direction,
								  float MoveDistance, UAnimMontage*& MontageToPlay, FName&         SectionToPlay);

	/* TraceWall을 실행하기 전에 이 로직을 실행
	 @param Direction 현재 캐릭터가 가고자 하는 방향
	 @return 현재 바라보는 방향과 인풋 방향이 지정된 각도 이내에 포함되는지 */
	bool CanTraceWall(const FVector& Direction) const;
	/* 전체적인 Trace를 실행 (코어 로직) */
	bool TraceWall(const FVector&         Position, const FVector&       Direction, float Height, int Iteration,
				   EParkourWallHitResult& WallHitResult, FWallHitResult& FinalResult) const;
	/* 파쿠르 액션 실행 */
	bool TryTakeParkourAction();

	void TryMoveOnLedge(const FVector2D& Direction);

	bool CanMoveOnLedge(const FVector2D& Direction);

	void MoveOnLedge(float MoveAmount, FVector& Location, FRotator& Rotation);

	void FindLedgeData(float          MoveAmount, const FVector& LedgePosition, const FVector& LedgeNormal,
					   const FVector& HitNormal, FVector&        NewLocation, FRotator&        NewRotator) const;

	bool FindAnimSets(const FWallHitResult& WallHitResult, EParkourWallHitResult ActionType,
					  FParkourAnim&         ParkourAnim);

	/* 난간에 발을 디딜 수 있는 표면이 있는지 확인 */
	bool CheckLedgeHasSurface();


	bool StartMotionWarping(const FWallHitResult& WallHitResult, EParkourWallHitResult ActionType);
	bool StartMotionWarpingWithDirection(const FWallHitResult& WallHitResult, const EMovementDirection Direction);

	void SetParkourState(const EParkourState TargetParkourState);
	void SetRotationValues(const FRotator& Rotator) const;
	bool TryHop();

	static bool    IsValueInRange(const FInt32Range& Range, const int Value);
	static FVector ApplyOffset(const FRotator& Rotator, const FVector& Vector);

	void ResetMotionWarping() const;

protected:
#pragma region Component
	UPROPERTY(BlueprintReadWrite, Category = "Component")
	class UMotionWarpingComponent* MotionWarpingComponent;
	UPROPERTY(BlueprintReadWrite, Category = "Component")
	class ULocomotionComponent* LocomotionComponent;
	UPROPERTY(BlueprintReadWrite, Category = "Component")
	class USplineComponent* SplinePath;
#pragma endregion

#pragma region Trace Hit
	UPROPERTY(BlueprintReadWrite, Category = "Trace Hit")
	FWallHitResult CachedWallHitResult;;
	UPROPERTY(BlueprintReadWrite, Category = "Trace Hit")
	float WallTrace_Height = 300.f;
	UPROPERTY(BlueprintReadWrite, Category = "Trace Hit")
	float WallTrace_Width = 10.f;
	UPROPERTY(BlueprintReadWrite, Category = "Trace Hit")
	float WallTrace_Distance = 150.f;
	UPROPERTY(BlueprintReadWrite, Category = "Trace Hit")
	float WallTrace_AngleThreshold = 135.f;
	UPROPERTY(BlueprintReadWrite, Category = "Trace Hit")
	int WallTrace_Iteration = 30;
	UPROPERTY(BlueprintReadWrite, Category = "Trace Hit")
	float Vault_DepthThreshold = 300.f;
	UPROPERTY(BlueprintReadWrite, Category = "Trace Hit")
	float Vault_MaxDepth = 80.f;
#pragma endregion

#pragma region State
	UPROPERTY(BlueprintReadWrite, Category = "State")
	EParkourState CurrentParkourState = EParkourState::None;
	UPROPERTY(BlueprintReadWrite, Category = "State")
	EParkourState PreviousParkourState = EParkourState::None;
	UPROPERTY(BlueprintReadWrite, Category = "State")
	bool bIsMoving = false;
	UPROPERTY(BlueprintReadWrite, Category = "State")
	EWallRunState WallRunState = EWallRunState::None;

#pragma endregion

	UPROPERTY(BlueprintReadWrite, Category = "Input")
	bool bJumpActionTriggered;

	UPROPERTY(BlueprintReadWrite, Category = "Input")
	bool bCanDodge = true;

	UPROPERTY(BlueprintReadWrite, Category = "Anim")
	UAnimMontage* CachedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bEnableDebug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimSet")
	TArray<UParkourAction*> ParkourDataAssets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimSet")
	TMap<EMovementDirection, UParkourAction*> ParkourDataAsset_Direction;

	/* 왼쪽 발 아래 난간이 막혀있는지 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ledge")
	bool bHasLedgeSurface_L;
	/* 오른쪽 발 아래 난간이 막혀있는지 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ledge")
	bool bHasLedgeSurface_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ledge")
	EMovementDirection LedgeMovementDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge&Roll")
	UAnimMontage* PerfectDodgeMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge&Roll")
	UAnimMontage* RollMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge&Roll")
	UAnimMontage* DodgeMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge&Roll")
	UAnimMontage* JXSQDodgeMontage;


	friend ULocomotionComponent;
};
