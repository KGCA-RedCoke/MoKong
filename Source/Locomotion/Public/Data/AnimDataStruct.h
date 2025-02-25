// copyright Jacob Lim

#pragma once

#include "CoreMinimal.h"
#include "AnimDataStruct.generated.h"

class UCharacterMovementComponent;
class UCapsuleComponent;

/* ============================================= Enums ================================================ */

UENUM(BlueprintType)
enum class EMovementDirection : uint8
{
	Forward,
	Backward,
	Left,
	Right
};

UENUM(BlueprintType)
enum class EInputHandleType : uint8
{
	Hold,
	Toggle
};

UENUM(BlueprintType)
enum class EInputResult : uint8
{
	Started,
	Tapped,
	Held,
	HeldReleased
};

UENUM(BlueprintType)
enum class EMovementType : uint8
{
	Ground,
	Swim,
	Fly,
	LedgeOrHang,
	Parkour,
	Slide,
	Cover,
	Ride
};

UENUM(BlueprintType)
enum class ERotationMode : uint8
{
	None,
	VelocityDirection,
	LookingDirection
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Stable,
	InAir,
	Falling
};

UENUM(BlueprintType)
enum class ELocomotionStances : uint8
{
	None,
	Neutral,
	Crouch,
	Balanced
};

UENUM(BlueprintType)
enum class EActionProfile : uint8
{
	None,
	Low,
	Mid,
	High
};

UENUM(BlueprintType)
enum class EOverlayStates: uint8
{
	None,
	Default,
	Injured,
	Tired,
	Hit,
	Interaction1,
	Interaction2,
	Interaction3,
	Interaction4,
	Interaction5,
};

UENUM(BlueprintType)
enum class EMovementModel : uint8
{
	None,
	Normal,
	Sluggish,
	Responsive
};

UENUM(BlueprintType)
enum class EPose : uint8
{
	None,
	Neutral,
	Aiming
};

UENUM(BlueprintType)
enum class ELocomotionMode : uint8
{
	Default,
	Boxer,
	Sword,
	LongSword,
	GreatSword,
	Halberd,
	Axe,
	Hammer,
	Bow
};


/* ============================================= Parkour ================================================ */

UENUM(BlueprintType)
enum class EWallRunState : uint8
{
	None,
	StartedToRun,
	GoingApex,
	Apex
};

UENUM(BlueprintType)
enum class EParkourMotionWarpingTarget : uint8
{
	WallTopStart,
	WallTopEnd,
	WallLand,
	WallStartHeight
};

UENUM(BlueprintType)
enum class EParkourType : uint8
{
	Default,
	Reach,
	Catch
};

UENUM(BlueprintType)
enum class EParkourState : uint8
{
	None,
	WallRun,
	StepUp,
	StepDown,
	Vault,
	Mantle,
	Slide,
	Hang,
	Ledge,
	Dodge
};

UENUM(BlueprintType)
enum class EParkourWallHitResult : uint8
{
	None,
	WallRun,
	SlideUnder,
	Step,
	Vault,
	Mantle
};

UENUM(BlueprintType)
enum class EDodgeType : uint8
{
	Dodge,
	PerfectDodge,
	Roll,
	JXSQ
};

/* ============================================= Struct ================================================ */

USTRUCT(BlueprintType)
struct FVelocityBlend
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Forward = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Backward = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Left = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Right = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Up = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Down = 0.f;
};

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACharacter* Character = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimInstance* AnimationInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCharacterMovementComponent* MovementComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D DefaultCapsuleSize = {0.f, 0.f};
};

USTRUCT(BlueprintType)
struct FMovementData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Acceleration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Deceleration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Friction = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationRate = 0.f;
};

/* Profile에 따라서 속도, 회전등을 전환 */
USTRUCT(BlueprintType)
struct FMovementByProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EActionProfile, FMovementData> MovementDataByProfile;
};

USTRUCT(BlueprintType)
struct FMovementByModel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERotationMode ForcedRotationMode = ERotationMode::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EMovementModel, FMovementByProfile> MovementDataByModel;
};

USTRUCT(BlueprintType)
struct FMovementByPose
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EPose, FMovementByModel> MovementDataByPose;
};

USTRUCT(BlueprintType)
struct FMovementByStance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ELocomotionStances, FMovementByPose> MovementDataByStance;
};

USTRUCT(BlueprintType)
struct FMovementByOverlay
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EOverlayStates, FMovementByStance> MovementDataByOverlay;
};

USTRUCT(BlueprintType)
struct FIdleSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* Idle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CustomFrame = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IdleBreakEntrySeconds = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimSequence*> IdleBreaks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ELocomotionStances, UAnimSequence*> EnterTransitionByStance;
};

USTRUCT(BlueprintType)
struct FAnimationSets
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ELocomotionStances, FIdleSet> IdleSetup;
};

USTRUCT(BlueprintType)
struct FTurnInPlaceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* Sequence = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TargetRotation = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayRateScale = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bScaleTargetAngle = false;
};

USTRUCT(BlueprintType)
struct FTurnInPlaceDataArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTurnInPlaceData> Variations;
};

USTRUCT(BlueprintType)
struct FWallHitResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLandable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bVaultable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FirstPosition = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DepthPosition = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FloorPosition = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector WallNormal = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Height = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Depth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BottomEmptyHeight = 0;
};

USTRUCT(BlueprintType)
struct FMotionWarpingSetup
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MotionWarpingTarget = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LocalSpaceOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EParkourMotionWarpingTarget Space = EParkourMotionWarpingTarget::WallTopStart;
};

USTRUCT(BlueprintType)
struct FParkourAnim
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* MontageToPlay = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMotionWarpingSetup> MotionWarpingSetups;
};

USTRUCT(BlueprintType)
struct FParkourRangeSetup
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInt32Range HeightRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInt32Range DepthRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInt32Range DistanceRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInt32Range VelocityRange;
};

USTRUCT(BlueprintType)
struct FComponentWithTransform
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPrimitiveComponent* Component = nullptr;
};
