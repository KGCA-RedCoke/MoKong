// copyright Jacob Lim

#pragma once

#include "CoreMinimal.h"
#include "LocomotionComponent_Base.h"
#include "LocomotionComponent.generated.h"

class ULandAction;
class UParkourComponent;
class UAnimInstance_Locomotion;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMovementTypeChanged,
											 const EMovementType,
											 MovementType,
											 const EMovementType,
											 Previous);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class LOCOMOTION_API ULocomotionComponent : public ULocomotionComponent_Base
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	float SelectVelocitySpeed() const;

#pragma region Main Function
	/* 회전값 초기화, 모든 locomotion 데이터 초기화 */
	void ResetDefaults();
	/* 초기화 후 locomotion 데이터를 업데이트 */
	void ConstructCharacterData();
#pragma endregion

#pragma region Update Functions

#pragma region Rotation
	void UpdateRotationData();
	void UpdateAirRotation();
	void UpdateRotationRateByState();
	void UpdateRotationByTurnInPlace();
	void UpdateCharacterRotationLimited(const float YawMin, const float YawMax, const float InterpSpeed);

#pragma region MovementType
	void UpdateGroundRotation();
	void UpdateSwimmingRotation();
	void UpdateFlyingRotation();
	void UpdateWallClimbRotation();
#pragma endregion
#pragma endregion

#pragma region Movement
	void           UpdateMovementData();
	EActionProfile UpdateDynamicProfile();
	void           UpdateDynamicMovementSettings();
	void           UpdateGaitAlpha();
#pragma endregion

#pragma region InAir
	/* 공중 상태에서의 데이터 업데이트 (최대 공중 지점, 캐릭터 상태  ... )*/
	void UpdateFallData();

	/* (지면이 아닌 상태에서 실행됨)
	 * 캐릭터 위치에서 땅을 향해 LineTrace 실행하고 처음 Hit된 오브젝트의 거리를 GroundDistance로 업데이트*/
	void GroundTrace();
#pragma endregion

#pragma region IK
	void UpdateFootIKValues();
	void UpdateFootLock(const FName& EnableFootIKCurve, const FName& FootLockCurve, const FName& FootIK_Bone,
						float&       CurrentAlpha, FVector&          CurrentLocation, FRotator&  CurrentRotation) const;
#pragma endregion

#pragma region Input
	/* 입력이 있으면 계속 더해지고 없으면 0으로 초기화 */
	void UpdateInputHeldDuration();
#pragma endregion

	/* MovementType에 따라서 Rotation을 진행 */
	void UpdateCharacterRotation();
	/* 데이터 조회, 검증 후 캐릭터 무브먼트 값 설정 */
	void UpdateCharacterMovement();

#pragma endregion

#pragma region Calculations
	FVector        CalculateRelativeAcceleration() const;
	static FVector CalculateJumpForce(const FVector& Direction, const float Gravity, const float JumpHeight,
									  const float    JumpDistance);
	float          CalculateFinalRotationRate() const;
	float          MapValuesToSpeedMap(TMap<EActionProfile, float>&& MapToBlend, const bool bClampMin);
	float          AdjustSpeedBySlope(const float TargetSpeed) const;
	float          InputHeldDurationMultiplier(const float Value, const float ClampedMin, const float MapRange) const;
	float          SelectVelocitySpeed();
	static FVector SafeDivideVector(const FVector& InVector, const float Divide);
#pragma endregion

#pragma region Setter

public:
#pragma region State
	UFUNCTION(BlueprintCallable)
	void SetDesiredGait(const EActionProfile TargetGaitProfile);
	UFUNCTION(BlueprintCallable)
	void SetRotationMode(const ERotationMode TargetRotationMode);
	UFUNCTION(BlueprintCallable)
	void SetStance(const ELocomotionStances TargetStance);
	UFUNCTION(BlueprintCallable)
	void SetOverlay(const EOverlayStates TargetOverlay);
	UFUNCTION(BlueprintCallable)
	void SetPose(const EPose TargetPose);
	UFUNCTION(BlueprintCallable)
	void SetMovementModel(const EMovementModel TargetMovementModel);
	UFUNCTION(BlueprintCallable)
	void SetMovementType(const EMovementType TargetMovementMode);

public:
	UFUNCTION(BlueprintCallable)
	void SetLocomotionMode(const ELocomotionMode TargetLocomotionMode);
#pragma endregion

#pragma region IK

protected:
	void SetFootLockOffset(FVector& Local_Location, FRotator& Local_Rotation) const;
	/* 전체적인 발 위치를 조정 */
	void SetFootOffset(const FName& EnableFootIKCurve, const FName&   FootBone, const FName& RootBone,
					   FVector&     Current_Location_Target, FVector& Current_Location_Offset,
					   FRotator&    Current_Rotation_Offset) const;
	/* 골반 높이를 발 위치에 따라서 조정 */
	void SetPelvisIKOffset(const FVector& FootOffset_L_Target, const FVector& FootOffset_R_Target);

#pragma endregion

	void SetCharacterRotation(const FRotator& TargetRotation, const float InterpSpeedConstant, const float InterpSpeed);
	void SetCharacterMovementData() const;
	void SetSelectedMovementData();
#pragma endregion

#pragma region Getter

protected:
	void GetMovementDataByProfile(const EActionProfile TargetProfile, FMovementData& OutMovementData,
								  EActionProfile&      OutProfile);
	EActionProfile   GetAllowedProfile();
	EActionProfile   GetActualProfile(const EActionProfile AllowedProfile);
	float            GetAnimCurveValue(const FName& CurveName) const;
	FTurnInPlaceData GetTurnInPlaceData(const float TurnAngle);

public:
	bool                        IsCrouching() const;
	float                       GetMovementSpeed() const;
	FORCEINLINE EActionProfile  GetCurrentGaitProfile() const { return CurrentGaitProfile; };
	FORCEINLINE EActionProfile  GetDesiredGaitProfile() const { return DesiredGaitProfile; };
	FORCEINLINE ELocomotionMode GetCurrentLocomotionMode() const { return CurrentLocomotionMode; };
	FORCEINLINE EPose           GetCurrentPose() const { return CurrentPose; };
	FORCEINLINE ERotationMode   GetCurrentRotationMode() const { return CurrentRotationMode; };


#pragma endregion

#pragma region Checks

protected:
	bool CanEnterHighProfile() const;
	bool CanUpdateMovingRotation() const;
	bool CanTurnInPlace() const;
#pragma endregion

#pragma region Input Handler

#pragma region MovementType
	void HandleGround() const;
	void HandleSwimming() const;
	void HandleWallClimbing();
	void HandleFlying() const;
#pragma endregion

public:
#pragma region Actions
	UFUNCTION(BlueprintCallable)
	void HandleJumpAction(const bool bAction, const float ElapsedTime);
	UFUNCTION(BlueprintCallable)
	void HandleCrouchAction(const bool bAction, const float ElapsedTime);
	UFUNCTION(BlueprintCallable)
	void HandleLowProfileAction(const bool bAction, const float ElapsedTime);
	UFUNCTION(BlueprintCallable)
	void HandleHighProfileAction(const bool bAction, const float ElapsedTime);
	UFUNCTION(BlueprintCallable)
	void HandleSecondaryProfileAction(const EInputResult InputResult, bool bAction, const float ElapsedTime);
#pragma endregion

	UFUNCTION(BlueprintCallable)
	void HandleMovementInput(const FVector2D Axis2D);
	UFUNCTION(BlueprintCallable)
	void HandleLookInput(const FVector2D Axis2D) const;

#pragma endregion

#pragma region Validation
	EOverlayStates     ValidateOverlayForMovementData(const EOverlayStates TargetOverlay);
	ELocomotionStances ValidateStanceForMovementData(const ELocomotionStances TargetStance);
	EMovementModel     ValidateMovementModelForMovementData(const EMovementModel TargetMovementModel);
	EActionProfile     ValidateGaitProfileForMovementData(const EActionProfile TargetProfile);
	EPose              ValidatePoseForMovementData(const EPose TargetPose);
#pragma endregion

#pragma region Callback
	virtual void OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode,
									   uint8       PreviousCustomMode) override;
	virtual void OnCharacterMovementUpdated(float DeltaSeconds, FVector OldLocation, FVector OldVelocity) override;
	virtual void OnCharacterLanded(const FHitResult& Hit) override;
	virtual void OnMontageBlendOut(UAnimMontage* Montage, bool bInterrupted) override;
	virtual void OnReachedApex() override;

	UFUNCTION()
	void OnMovementTypeUpdated(const EMovementType TargetMovementType, const EMovementType Previous);

	/* 착지 후 0.2초 이후에 점프가 가능하도록 설정 및 타이머 초기화 */
	UFUNCTION()
	void ActivateJumpAfterLand();
#pragma endregion

	bool CircularDetection(const FVector&      Position, const FVector& Direction, const int Iteration, const float Radius,
						   const float         SphereRadius, const float Height, const float Degrees, const float Offset,
						   const FLinearColor& TraceColor, TArray<FHitResult>& AvailablePoints,
						   TArray<FHitResult>& AllPoints) const;

public:
	FOnMovementTypeChanged OnMovementTypeChanged;

protected:
#pragma region Action Handle Type
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|ActionHandleType")
	EInputHandleType CrouchAction_HandleType;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|ActionHandleType")
	EInputHandleType WalkAction_HandleType;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|ActionHandleType")
	EInputHandleType SprintAction_HandleType;
#pragma endregion

#pragma region Debug
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Debug")
	bool bEnableDebug;
#pragma endregion

#pragma region Data

#pragma region Main
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|Main")
	float ComponentDeltaSeconds;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|Main")
	float InputHeldDuration;
#pragma endregion

#pragma region Rotation Data
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|RotationData")
	float AimYawRate;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|RotationData")
	float PreviousAimYaw;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|RotationData")
	float AimingAngle;

	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|RotationData")
	FRotator ControlRotation;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|RotationData")
	FRotator LastInputRotation;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|RotationData")
	FRotator LastVelocityRotation;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|RotationData")
	FRotator LastAirRotation;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|RotationData")
	FRotator CharacterSmoothRotation;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|RotationData")
	FRotator CharacterLastTargetRotation;
#pragma endregion

#pragma region MovementData
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|MovementData")
	bool bHasMovementInput;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|MovementData")
	bool bIsMoving;

	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|MovementData")
	float MovementSpeed;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|MovementData")
	float MovementInputAlpha;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|MovementData")
	float FloorDotProduct;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|MovementData")
	float GaitAlpha;

	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|MovementData")
	FVector2D MovementInput;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|MovementData")
	FVector CharacterOldVelocity;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|MovementData")
	FVector PhysicalAcceleration;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|MovementData")
	FVector RelativeAcceleration;

	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|MovementData")
	FMovementData CurrentMovementData;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|MovementData")
	FMovementByOverlay SelectedMovementData;
#pragma endregion

#pragma region States
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|States")
	EMovementType CurrentMovementType;

	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|States")
	EMovementModel CurrentMovementModel;

	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|States")
	ELocomotionStances CurrentStance;

	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|States")
	EOverlayStates CurrentMovementOverlay;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|States")
	EOverlayStates DesiredMovementOverlay;

	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|States")
	EPose CurrentPose;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|States")
	EPose DesiredPose;

	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|States")
	EActionProfile CurrentGaitProfile;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|States")
	EActionProfile DesiredGaitProfile;

	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|States")
	ERotationMode CurrentRotationMode;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|States")
	ERotationMode DesiredRotationMode;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|States")
	ERotationMode ForcedRotationMode;

	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|States")
	ECharacterState CurrentCharacterState;

	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|States")
	TEnumAsByte<EMovementMode> CurrentMovementMode;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|States")
	TEnumAsByte<EMovementMode> PreviousMovementMode;

	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|States")
	ELocomotionMode CurrentLocomotionMode;
#pragma endregion

#pragma region IK

	UPROPERTY(BlueprintReadWrite, Category = "IK|FootLock")
	float FootLock_R_Alpha;

	UPROPERTY(BlueprintReadWrite, Category = "IK|FootLock")
	float FootLock_L_Alpha;

	UPROPERTY(BlueprintReadWrite, Category = "IK|FootLock")
	FVector FootLock_R_Location;

	UPROPERTY(BlueprintReadWrite, Category = "IK|FootLock")
	FVector FootLock_L_Location;

	UPROPERTY(BlueprintReadWrite, Category = "IK|FootLock")
	FRotator FootLock_R_Rotation;

	UPROPERTY(BlueprintReadWrite, Category = "IK|FootLock")
	FRotator FootLock_L_Rotation;

	UPROPERTY(BlueprintReadWrite, Category = "IK|Foot IK")
	FVector FootTarget_R;

	UPROPERTY(BlueprintReadWrite, Category = "IK|Foot IK")
	FVector FootTarget_L;

	UPROPERTY(BlueprintReadWrite, Category = "IK|Foot IK")
	FVector FootOffset_R;

	UPROPERTY(BlueprintReadWrite, Category = "IK|Foot IK")
	FVector FootOffset_L;

	UPROPERTY(BlueprintReadWrite, Category = "IK|Foot IK")
	FRotator FootRotationOffset_R;

	UPROPERTY(BlueprintReadWrite, Category = "IK|Foot IK")
	FRotator FootRotationOffset_L;

	UPROPERTY(BlueprintReadWrite, Category = "IK|Foot IK")
	float PelvisAlpha;

	UPROPERTY(BlueprintReadWrite, Category = "IK|Foot IK")
	FVector PelvisOffset;


#pragma endregion

#pragma region Falling
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|Falling")
	float GroundDistance;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|Falling")
	float ApexDistance;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|Falling")
	float TimeToJumpApex;
#pragma endregion

#pragma region Turn In Place
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|Turn In Place")
	float TurnInPlaceElapsedDelayTime;
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent|Data|Turn In Place")
	float TurnInPlaceRotationScale;
#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocomotionComponent|Data")
	ELocomotionStances Stance = ELocomotionStances::Neutral;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocomotionComponent|Data")
	ERotationMode RotationMode = ERotationMode::LookingDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocomotionComponent|Data")
	EActionProfile GaitProfile = EActionProfile::Mid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocomotionComponent|Data")
	EOverlayStates OverlayState = EOverlayStates::Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocomotionComponent|Data")
	EMovementModel MovementModel = EMovementModel::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocomotionComponent|Data")
	FVector2D LookInputSensitivity = {30, 30};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocomotionComponent|Data")
	FVector2D SlopeWalkSpeedClamp = {100, 1000};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocomotionComponent|Data")
	float TurnInPlaceThreshold = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocomotionComponent|Data")
	TMap<EMovementType, FMovementByOverlay> MovementData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocomotionComponent|Data")
	TMap<EMovementType, FAnimationSets> AnimationSets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocomotionComponent|Data")
	TMap<ELocomotionStances, FTurnInPlaceDataArray> TurnInPlaceByStance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LocomotionComponent|Data")
	TMap<ELocomotionMode, TSubclassOf<UAnimInstance>> LocomotionLayerSets;
#pragma endregion

	UPROPERTY()
	UParkourComponent* ParkourComponent;

	UPROPERTY(BlueprintReadWrite)
	bool bJumpActionPressed;
	UPROPERTY(BlueprintReadWrite)
	bool bHighProfileActionPressed;
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle JumpActivatorTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimSet")
	TArray<ULandAction*> LandDataAssets;

	friend UParkourComponent;
};
