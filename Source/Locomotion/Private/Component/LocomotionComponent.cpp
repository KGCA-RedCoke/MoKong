// copyright Jacob Lim

#include "Component/LocomotionComponent.h"

#include "Component/ParkourComponent.h"
#include "Components/CapsuleComponent.h"
#include "Data/Asset/LandAction.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void ULocomotionComponent::BeginPlay()
{
	Super::BeginPlay();

	ConstructCharacterData();
}

void ULocomotionComponent::ResetDefaults()
{
	CurrentMovementType = EMovementType::Ground;

	GetMovementComponent()->bUseControllerDesiredRotation = false;
	GetMovementComponent()->bOrientRotationToMovement     = false;
	GetCharacter()->bUseControllerRotationYaw             = false;

	CharacterSmoothRotation     = GetCharacter()->GetActorRotation();
	CharacterLastTargetRotation = CharacterSmoothRotation;
	LastAirRotation             = CharacterSmoothRotation;
	ControlRotation             = CharacterSmoothRotation;

	CurrentLocomotionMode = ELocomotionMode::Default;

	CurrentMovementOverlay = ValidateOverlayForMovementData(OverlayState);
	DesiredMovementOverlay = OverlayState == EOverlayStates::None ? EOverlayStates::Default : OverlayState;

	CurrentStance = ValidateStanceForMovementData(Stance);

	CurrentPose = EPose::Neutral;

	CurrentMovementModel = ValidateMovementModelForMovementData(MovementModel);

	DesiredGaitProfile = ValidateGaitProfileForMovementData(GaitProfile) == EActionProfile::High
							 ? EActionProfile::Mid
							 : ValidateGaitProfileForMovementData(GaitProfile);

	CurrentRotationMode = RotationMode == ERotationMode::None ? ERotationMode::VelocityDirection : RotationMode;
	DesiredRotationMode = CurrentRotationMode;

	CurrentCharacterState = GetMovementComponent()->IsFalling()
								? ECharacterState::Falling
								: ECharacterState::Stable;
}

void ULocomotionComponent::ConstructCharacterData()
{
	OnMovementTypeChanged.AddDynamic(this, &ThisClass::OnMovementTypeUpdated);

	ResetDefaults();
	SetSelectedMovementData();

	// Swimming, Flight, Traversal Manager 설정
	ParkourComponent = Cast<UParkourComponent>(GetOwner()->GetComponentByClass(UParkourComponent::StaticClass()));
}

void ULocomotionComponent::UpdateRotationData()
{
	if (bIsMoving)
	{
		LastVelocityRotation = UKismetMathLibrary::MakeRotFromX(GetCharacter()->GetVelocity());
	}

	if (bHasMovementInput)
	{
		LastInputRotation = UKismetMathLibrary::MakeRotFromX(CurrentAcceleration);
	}

	ControlRotation = GetCharacter()->GetControlRotation();

	AimYawRate     = abs(UKismetMathLibrary::SafeDivide(ControlRotation.Yaw - PreviousAimYaw, ComponentDeltaSeconds));
	PreviousAimYaw = ControlRotation.Yaw;

	AimingAngle = UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, CharacterLastTargetRotation).Yaw;

	UpdateRotationRateByState();
}

void ULocomotionComponent::UpdateAirRotation()
{
	switch (CurrentRotationMode)
	{
	case ERotationMode::None:
		return;
	case ERotationMode::VelocityDirection:
		SetCharacterRotation(FRotator(0, LastAirRotation.Yaw, 0), 800.f, CalculateFinalRotationRate());

		LastAirRotation = bIsMoving
							  ? UKismetMathLibrary::MakeRotFromX(GetCharacter()->GetVelocity())
							  : GetCharacter()->GetActorRotation();
		break;
	case ERotationMode::LookingDirection:
		SetCharacterRotation(FRotator(0, ControlRotation.Yaw, 0), 800.f, CalculateFinalRotationRate());
		break;
	}
}

void ULocomotionComponent::UpdateRotationRateByState()
{
	EActionProfile Low,     Mid,     High;
	FMovementData  LowData, MidData, HighData;

	GetMovementDataByProfile(EActionProfile::Low, LowData, Low);
	GetMovementDataByProfile(EActionProfile::Mid, MidData, Mid);
	GetMovementDataByProfile(EActionProfile::High, HighData, High);

	CurrentMovementData.RotationRate
			= MapValuesToSpeedMap(TMap<EActionProfile, float>{
									  {EActionProfile::Low, LowData.RotationRate},
									  {EActionProfile::Mid, MidData.RotationRate},
									  {EActionProfile::High, HighData.RotationRate}
								  },
								  true);
}

void ULocomotionComponent::UpdateRotationByTurnInPlace()
{
	if (abs(GetAnimCurveValue("RotationAmount")) > 0.001)
	{
		GetCharacter()->AddActorWorldRotation(
											  FRotator(
													   0.f,
													   GetAnimCurveValue("RotationAmount") * (ComponentDeltaSeconds /
														   UKismetMathLibrary::SafeDivide(1, 30)),
													   0.f));

		CharacterSmoothRotation     = GetCharacter()->GetActorRotation();
		CharacterLastTargetRotation = CharacterSmoothRotation;
	}
}

void ULocomotionComponent::UpdateCharacterRotationLimited(const float YawMin, const float YawMax,
														  const float InterpSpeed)
{
	const FRotator CharacterRot = GetCharacter()->GetActorRotation();
	const float    RotatorYaw   = UKismetMathLibrary::NormalizedDeltaRotator(
																			 ControlRotation,
																			 CharacterRot).Yaw;

	// UE_LOG(LogTemp, Warning, TEXT("CharacterRot: %f"), RotatorYaw);

	const float TargetYaw = RotatorYaw > 0.f ? ControlRotation.Yaw + YawMin : ControlRotation.Yaw + YawMax;

	if (!UKismetMathLibrary::InRange_FloatFloat(RotatorYaw, YawMin, YawMax, true, true))
	{
		SetCharacterRotation(FRotator(CharacterRot.Pitch, TargetYaw, CharacterRot.Roll), 1000.f, InterpSpeed);
		LastVelocityRotation.Yaw = TargetYaw;
	}
}

void ULocomotionComponent::UpdateGroundRotation()
{
	switch (CurrentCharacterState)
	{
	case ECharacterState::Stable:
		if (CanUpdateMovingRotation())
		{
			switch (CurrentRotationMode)
			{
			case ERotationMode::None:
				return;
			case ERotationMode::VelocityDirection:
				SetCharacterRotation(FRotator(0, LastVelocityRotation.Yaw, 0.f), 800.f, CalculateFinalRotationRate());
				break;
			case ERotationMode::LookingDirection:
				switch (CurrentGaitProfile)
				{
				case EActionProfile::Low:
				case EActionProfile::Mid:
					SetCharacterRotation(
										 FRotator(0.f, ControlRotation.Yaw + GetAnimCurveValue("YawOffset"), 0.f),
										 800.f,
										 CalculateFinalRotationRate());
					break;
				case EActionProfile::High:
					SetCharacterRotation(
										 FRotator(0.f, LastVelocityRotation.Yaw, 0.f),
										 500.f,
										 CalculateFinalRotationRate());
					break;
				default: ;
				}

				break;
			}
		}
		else
		{
			if (CurrentPose == EPose::Aiming)
			{
				UpdateCharacterRotationLimited(-100.f, 100.f, 20.f);

				UpdateRotationByTurnInPlace();
			}
		}
		break;
	case ECharacterState::InAir:
	case ECharacterState::Falling:
		UpdateAirRotation();
		break;
	}
}

void ULocomotionComponent::UpdateSwimmingRotation()
{}

void ULocomotionComponent::UpdateFlyingRotation()
{}

void ULocomotionComponent::UpdateWallClimbRotation()
{}

void ULocomotionComponent::UpdateMovementData()
{
	// Physical Acceleration (속도 변위 / 프레임 -> 가속도)
	PhysicalAcceleration = (GetCharacter()->GetVelocity() - CharacterOldVelocity) / ComponentDeltaSeconds;
	CharacterOldVelocity = GetCharacter()->GetVelocity();

	// Relative Acceleration (현재 속도에서 상대적인 가속도)
	RelativeAcceleration = CalculateRelativeAcceleration();

	// Speed (속도 (스칼라))
	MovementSpeed = SelectVelocitySpeed();

	// Is Moving?
	bIsMoving = MovementSpeed > 0.f;

	// Movement Input Amount (가속도 세기 (0 ~ 1))
	MovementInputAlpha = UKismetMathLibrary::SafeDivide(CurrentAcceleration.Length(),
														GetMovementComponent()->GetMaxAcceleration());
	// Has Movement? 
	bHasMovementInput = MovementInputAlpha > 0.f;

	// InputDuration 업데이트
	UpdateInputHeldDuration();

	// 이동중인 방향과 지면 노말의 Dot (지면이 평평하면 대부분 0)
	FloorDotProduct = GetMovementComponent()->GetLastUpdateVelocity().GetSafeNormal().Dot(
	 GetMovementComponent()->CurrentFloor.HitResult.ImpactNormal);

	// GaitProfile에 따른 데이터 업데이트 
	UpdateGaitAlpha();
}

EActionProfile ULocomotionComponent::UpdateDynamicProfile()
{
	const EActionProfile Local_AllowedProfile = GetAllowedProfile();
	const EActionProfile ActualProfile        = GetActualProfile(Local_AllowedProfile);

	CurrentGaitProfile = ActualProfile;

	return Local_AllowedProfile;
}

void ULocomotionComponent::UpdateDynamicMovementSettings()
{
	const EActionProfile Local_AllowedProfile = UpdateDynamicProfile();

	FMovementData  Local_MovementData;
	EActionProfile Local_Profile;
	GetMovementDataByProfile(Local_AllowedProfile, Local_MovementData, Local_Profile);

	CurrentMovementData.WalkSpeed = UKismetMathLibrary::Ease(
															 CurrentMovementData.WalkSpeed,
															 AdjustSpeedBySlope(Local_MovementData.WalkSpeed),
															 ComponentDeltaSeconds * 15.f,
															 EEasingFunc::SinusoidalIn);

	EActionProfile LowProfile, MidProfile, HighProfile;
	FMovementData  LowData,    MidData,    HighData;

	GetMovementDataByProfile(EActionProfile::Low, LowData, LowProfile);
	GetMovementDataByProfile(EActionProfile::Mid, MidData, MidProfile);
	GetMovementDataByProfile(EActionProfile::High, HighData, HighProfile);

	CurrentMovementData.Acceleration =
			InputHeldDurationMultiplier(
										UKismetMathLibrary::Ease(
																 CurrentMovementData.Acceleration,
																 MapValuesToSpeedMap(TMap<EActionProfile, float>
																	 {
																		 {EActionProfile::Low, LowData.Acceleration},
																		 {EActionProfile::Mid, MidData.Acceleration},
																		 {EActionProfile::High, HighData.Acceleration}
																	 },
																	 true),
																 ComponentDeltaSeconds * 20.f,
																 EEasingFunc::SinusoidalIn
																),
										100.f,
										0.01f);

	CurrentMovementData.Deceleration =
			UKismetMathLibrary::Ease(
									 CurrentMovementData.Deceleration,
									 MapValuesToSpeedMap(TMap<EActionProfile, float>
														 {
															 {EActionProfile::Low, LowData.Deceleration},
															 {EActionProfile::Mid, MidData.Deceleration},
															 {EActionProfile::High, HighData.Deceleration}
														 },
														 true),
									 ComponentDeltaSeconds * 20.f,
									 EEasingFunc::SinusoidalIn
									);

	CurrentMovementData.Friction =
			InputHeldDurationMultiplier(
										UKismetMathLibrary::Ease(
																 CurrentMovementData.Friction,
																 MapValuesToSpeedMap(TMap<EActionProfile, float>
																	 {
																		 {EActionProfile::Low, LowData.Friction},
																		 {EActionProfile::Mid, MidData.Friction},
																		 {EActionProfile::High, HighData.Friction}
																	 },
																	 true),
																 ComponentDeltaSeconds * 20.f,
																 EEasingFunc::SinusoidalIn
																),
										1.f,
										0.01f);

	SetCharacterMovementData();
}


void ULocomotionComponent::UpdateGaitAlpha()
{
	GaitAlpha = MapValuesToSpeedMap(
									TMap<EActionProfile, float>
									{
										{EActionProfile::Low, 1.f},
										{EActionProfile::Mid, 2.f},
										{EActionProfile::High, CanEnterHighProfile() ? 3.f : 2.f}
									},
									false);

	SetCharacterMovementData();
}

void ULocomotionComponent::UpdateFallData()
{
	if (CurrentMovementType == EMovementType::Ground ||
		CurrentMovementType == EMovementType::Parkour ||
		CurrentMovementType == EMovementType::Cover)
	{
		if (CurrentCharacterState != ECharacterState::Stable)
		{
			GroundTrace();

			if (CurrentCharacterState == ECharacterState::InAir)
			{
				// 최대 높이에 도달하기 위한 시간 계산
				TimeToJumpApex = UKismetMathLibrary::SafeDivide(
																-GetMovementComponent()->GetLastUpdateVelocity().
																Z, // 올라가는 속도를 계산하기 위해 -를 곱해준다.
																GetMovementComponent()->GetGravityZ());

				// Z 속도가 음수라면 아래로 떨어지고 있음..
				if (GetCharacter()->GetVelocity().Z <= 0)
				{
					CurrentCharacterState = ECharacterState::Falling;
				}
			}
			else if (CurrentCharacterState == ECharacterState::Falling)
			{
				// 최대 높이 도달 시간은 초기화
				TimeToJumpApex = 0.f;
			}
		}
	}
}

void ULocomotionComponent::GroundTrace()
{
	FHitResult    HitResult;
	const FVector TraceStart = GetSkeletalMeshComponent()->GetComponentLocation();
	const FVector TraceEnd   = TraceStart - FVector(0, 0, 500);
	float         TargetDistance;

	GetWorld()->LineTraceSingleByChannel(
										 HitResult,
										 TraceStart,
										 TraceEnd,
										 ECC_Visibility);

#ifdef UE_BUILD_DEBUG
	if (bEnableDebug)
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.f, 0.f, 1.f);
#endif


	if (HitResult.bBlockingHit)
	{
		TargetDistance = HitResult.Distance;
	}
	else
	{
		TargetDistance = (TraceStart - TraceEnd).Length();
	}

	GroundDistance = TargetDistance;
}

void ULocomotionComponent::UpdateFootIKValues()
{
	SetFootOffset("Enable_Foot_IK_L", "IK_Foot_L", "Root", FootTarget_L, FootOffset_L, FootRotationOffset_L);
	SetFootOffset("Enable_Foot_IK_R", "IK_Foot_R", "Root", FootTarget_R, FootOffset_R, FootRotationOffset_R);

	SetPelvisIKOffset(FootTarget_L, FootTarget_R);

	UpdateFootLock("Enable_Foot_IK_R",
				   "Foot_Lock_R",
				   "IK_Foot_R",
				   FootLock_R_Alpha,
				   FootLock_R_Location,
				   FootLock_R_Rotation);
	UpdateFootLock("Enable_Foot_IK_L",
				   "Foot_Lock_L",
				   "IK_Foot_L",
				   FootLock_L_Alpha,
				   FootLock_L_Location,
				   FootLock_L_Rotation);
}

void ULocomotionComponent::UpdateFootLock(const FName& EnableFootIKCurve, const FName& FootLockCurve,
										  const FName& FootIK_Bone, float&             CurrentAlpha,
										  FVector&     CurrentLocation, FRotator&      CurrentRotation) const
{
	if (GetAnimCurveValue(EnableFootIKCurve) > 0)
	{
		const float TargetFootLockAlpha = GetAnimCurveValue(FootLockCurve);

		if (TargetFootLockAlpha >= .99f || TargetFootLockAlpha < CurrentAlpha)
		{
			CurrentAlpha = TargetFootLockAlpha;
		}
		if (CurrentAlpha >= .99f)
		{
			CurrentLocation = GetSkeletalMeshComponent()->GetSocketTransform(FootIK_Bone, RTS_Component).GetLocation();
			CurrentRotation = GetSkeletalMeshComponent()->GetSocketTransform(FootIK_Bone, RTS_Component).GetRotation().
														  Rotator();
		}

		if (CurrentAlpha > 0.f)
		{
			SetFootLockOffset(CurrentLocation, CurrentRotation);
		}
	}
}

void ULocomotionComponent::UpdateInputHeldDuration()
{
	InputHeldDuration = bHasMovementInput ? InputHeldDuration + ComponentDeltaSeconds : 0;
}

void ULocomotionComponent::UpdateCharacterRotation()
{
	switch (CurrentMovementType)
	{
	case EMovementType::Ground:
	case EMovementType::Slide:
		UpdateGroundRotation();
		break;
	case EMovementType::Swim:
		UpdateSwimmingRotation();
		break;
	case EMovementType::Fly:
		UpdateFlyingRotation();
		break;
	case EMovementType::LedgeOrHang:
		UpdateWallClimbRotation();
		break;
	case EMovementType::Parkour:
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("아직 구현되지 않음 구현 필요 %hhd"), CurrentMovementType);
		break;;
	}
}


void ULocomotionComponent::UpdateCharacterMovement()
{
	UpdateDynamicMovementSettings();
}

FVector ULocomotionComponent::CalculateRelativeAcceleration() const
{
	// 가속이 붙었는지 확인
	const float Divider = PhysicalAcceleration.Dot(CharacterOldVelocity) > 0.f
							  ? CurrentMovementData.Acceleration
							  : CurrentMovementData.Deceleration;


	// 원래의 캐릭터 방향에 맞게 회전시킨 벡터
	return UKismetMathLibrary::Quat_UnrotateVector(
												   GetCharacter()->GetActorRotation().Quaternion(), // 캐릭터 방향
												   SafeDivideVector(PhysicalAcceleration.GetClampedToMaxSize(Divider),
																	Divider) // 조절된 각도, 크기 제한 후 Divider로 나눔
												  );
}

FVector ULocomotionComponent::CalculateJumpForce(const FVector& Direction, const float Gravity, const float JumpHeight,
												 const float    JumpDistance)
{
	const float PeakTime = FMath::Sqrt(FMath::Pow(JumpHeight, JumpHeight) / Gravity);

	const float   VerticalVelocity   = Gravity * PeakTime;
	const float   HorizontalVelocity = JumpDistance * PeakTime;
	const FVector AirVelocity        = Direction * HorizontalVelocity;

	return FVector(AirVelocity.X, AirVelocity.Y, VerticalVelocity);
}

float ULocomotionComponent::CalculateFinalRotationRate() const
{
	return CurrentMovementData.RotationRate * UKismetMathLibrary::MapRangeClamped(AimYawRate, 0, 300, 1, 3);
}

float ULocomotionComponent::MapValuesToSpeedMap(TMap<EActionProfile, float>&& MapToBlend, const bool bClampMin)
{
	EActionProfile LowProfile, MidProfile, HighProfile;
	FMovementData  LowData,    MidData,    HighData;

	GetMovementDataByProfile(EActionProfile::Low, LowData, LowProfile);
	GetMovementDataByProfile(EActionProfile::Mid, MidData, MidProfile);
	GetMovementDataByProfile(EActionProfile::High, HighData, HighProfile);

	const float LowValue = UKismetMathLibrary::MapRangeClamped(MovementSpeed,
															   0.f,
															   LowData.WalkSpeed,
															   bClampMin ? *MapToBlend.Find(EActionProfile::Low) : 0,
															   *MapToBlend.Find(EActionProfile::Low));

	const float MidValue = UKismetMathLibrary::MapRangeClamped(MovementSpeed,
															   LowData.WalkSpeed,
															   MidData.WalkSpeed,
															   *MapToBlend.Find(EActionProfile::Low),
															   *MapToBlend.Find(EActionProfile::Mid));

	const float HighValue = UKismetMathLibrary::MapRangeClamped(MovementSpeed,
																MidData.WalkSpeed,
																HighData.WalkSpeed,
																*MapToBlend.Find(EActionProfile::Mid),
																*MapToBlend.Find(EActionProfile::High));
	return MovementSpeed > MidData.WalkSpeed ? HighValue : (MovementSpeed > LowData.WalkSpeed ? MidValue : LowValue);
}

float ULocomotionComponent::AdjustSpeedBySlope(const float TargetSpeed) const
{
	return (CurrentMovementType == EMovementType::Ground || CurrentMovementType == EMovementType::Slide)
			   ? UKismetMathLibrary::Clamp(
										   UKismetMathLibrary::Lerp(TargetSpeed, 0.f, -FloorDotProduct),
										   SlopeWalkSpeedClamp.X,
										   SlopeWalkSpeedClamp.Y)
			   : TargetSpeed;
}

float ULocomotionComponent::InputHeldDurationMultiplier(const float Value, const float ClampedMin,
														const float MapRange) const
{
	return FMath::Max(
					  Value * UKismetMathLibrary::MapRangeClamped(InputHeldDuration, 0.f, MapRange, 0.f, 1.f),
					  ClampedMin);
}

float ULocomotionComponent::SelectVelocitySpeed()
{
	switch (CurrentMovementMode)
	{
	case MOVE_None:
	case MOVE_Walking:
	case MOVE_NavWalking:
	case MOVE_Falling:
	case MOVE_Custom:
		return GetCharacter()->GetVelocity().Size2D();
	case MOVE_Swimming:
	case MOVE_Flying:
		return GetCharacter()->GetVelocity().Size();
	default: ;
	}

	return 0.f;
}

FVector ULocomotionComponent::SafeDivideVector(const FVector& InVector, const float Divide)
{
	return FVector(
				   UKismetMathLibrary::SafeDivide(InVector.X, Divide),
				   UKismetMathLibrary::SafeDivide(InVector.Y, Divide),
				   UKismetMathLibrary::SafeDivide(InVector.Z, Divide)
				  );
}

void ULocomotionComponent::SetDesiredGait(const EActionProfile TargetGaitProfile)
{
	DesiredGaitProfile = TargetGaitProfile;
}

void ULocomotionComponent::SetRotationMode(const ERotationMode TargetRotationMode)
{
	DesiredRotationMode = TargetRotationMode;

	CurrentRotationMode = ForcedRotationMode == ERotationMode::None ? DesiredRotationMode : ForcedRotationMode;
}

void ULocomotionComponent::SetStance(const ELocomotionStances TargetStance)
{
	if (TargetStance == ELocomotionStances::Neutral)
	{
		if (GetMovementComponent()->IsCrouching())
		{
			const FVector HeadLoc = GetSkeletalMeshComponent()->GetSocketLocation("head");

			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(GetOwner());
			FHitResult HitResult;

			UKismetSystemLibrary::LineTraceSingle(
												  GetWorld(),
												  HeadLoc + FVector::UpVector * 10.f,
												  HeadLoc + FVector::UpVector * GetCapsuleComponent()->
												  GetUnscaledCapsuleHalfHeight() * 1.5f,
												  TraceTypeQuery1,
												  false,
												  ActorsToIgnore,
												  EDrawDebugTrace::ForOneFrame,
												  HitResult,
												  true);

			if (!HitResult.bBlockingHit)
			{
				GetCharacter()->UnCrouch();

				CurrentStance = ValidateStanceForMovementData(TargetStance);
				SetPose(CurrentPose);
			}
		}
	}
	else if (TargetStance == ELocomotionStances::Crouch)
	{
		if (CurrentCharacterState == ECharacterState::Stable && GetCharacter()->CanCrouch())
		{
			GetCharacter()->Crouch();

			CurrentStance = ValidateStanceForMovementData(TargetStance);
			SetPose(CurrentPose);
		}
	}
}

void ULocomotionComponent::SetOverlay(const EOverlayStates TargetOverlay)
{
	DesiredMovementOverlay = TargetOverlay;

	CurrentMovementOverlay = ValidateOverlayForMovementData(DesiredMovementOverlay);

	// Stance 초기화
	SetStance(CurrentStance);
}

void ULocomotionComponent::SetPose(const EPose TargetPose)
{
	CurrentPose = ValidatePoseForMovementData(TargetPose);

	DesiredPose = TargetPose;

	// MovementModel 초기화
	SetMovementModel(CurrentMovementModel);

	ForcedRotationMode = MovementData.FindChecked(CurrentMovementType).
									  MovementDataByOverlay.FindChecked(CurrentMovementOverlay).
									  MovementDataByStance.FindChecked(CurrentStance).
									  MovementDataByPose.FindChecked(CurrentPose).
									  ForcedRotationMode;

	SetRotationMode(ForcedRotationMode);
}

void ULocomotionComponent::SetMovementModel(const EMovementModel TargetMovementModel)
{
	CurrentMovementModel = ValidateMovementModelForMovementData(TargetMovementModel);
}

void ULocomotionComponent::SetMovementType(const EMovementType TargetMovementMode)
{
	const EMovementType PreviousMode = CurrentMovementType;

	CurrentMovementType = TargetMovementMode;

	OnMovementTypeChanged.Broadcast(TargetMovementMode, PreviousMode);

	if (MovementData.Contains(TargetMovementMode))
	{
		SetSelectedMovementData();
	}

	// Overlay 초기화
	SetOverlay(DesiredMovementOverlay);
}

void ULocomotionComponent::SetLocomotionMode(const ELocomotionMode TargetLocomotionMode)
{
	if (CurrentLocomotionMode != TargetLocomotionMode)
	{
		if (LocomotionLayerSets.Contains(TargetLocomotionMode))
		{
			GetAnimInstance()->LinkAnimClassLayers(LocomotionLayerSets[TargetLocomotionMode]);


			/*if (!GetAnimInstance()->IsAnyMontagePlaying())
			{
				GetAnimInstance()->Montage_Play(TargetLocomotionMode == ELocomotionMode::Default
					                                ? UnEquipMontageSets[CurrentLocomotionMode]
					                                : EquipMontageSets[TargetLocomotionMode]);
			}*/

			CurrentLocomotionMode = TargetLocomotionMode;

			SetOverlay(CurrentMovementOverlay);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT(" Locomotion Sets에 데이터 넣어야함. "));
		}
	}
}

void ULocomotionComponent::SetFootLockOffset(FVector& Local_Location, FRotator& Local_Rotation) const
{
	if (CurrentCharacterState == ECharacterState::Stable)
	{
		const FRotator Local_RotationOffset = UKismetMathLibrary::NormalizedDeltaRotator(
		 GetCharacter()->GetActorRotation(),
		 GetMovementComponent()->GetLastUpdateRotation());

		const FVector Local_LocationOffset = UKismetMathLibrary::Quat_UnrotateVector(
		 GetSkeletalMeshComponent()->GetComponentRotation().Quaternion(),
		 CharacterOldVelocity * ComponentDeltaSeconds);

		Local_Location = UKismetMathLibrary::RotateAngleAxis(Local_Location - Local_LocationOffset,
															 Local_RotationOffset.Yaw,
															 FVector(0, 0, -1));

		Local_Rotation = UKismetMathLibrary::NormalizedDeltaRotator(Local_Rotation, Local_RotationOffset);
	}
}

void ULocomotionComponent::SetFootOffset(const FName& EnableFootIKCurve, const FName&   FootBone, const FName& RootBone,
										 FVector&     Current_Location_Target, FVector& Current_Location_Offset,
										 FRotator&    Current_Rotation_Offset) const
{
	const FVector FootLocation = GetSkeletalMeshComponent()->GetSocketLocation(FootBone);
	const FVector RootLocation = GetSkeletalMeshComponent()->GetSocketLocation(RootBone);

	// 발위치 (X, y) + 루트 위치 (Z) => 각 발의 땅바닥 위치
	const FVector FootFloorLocation = FVector(FootLocation.X, FootLocation.Y, RootLocation.Z);

	// 커브값이 존재할때만 실행
	if (GetAnimCurveValue(EnableFootIKCurve) > 0)
	{
		FHitResult      HitResult;
		TArray<AActor*> ActorsToIgnore;

		// 땅바닥의 약간 위에서부터 아래로 SphereTrace 실행
		UKismetSystemLibrary::SphereTraceSingle(
												GetWorld(),
												FootFloorLocation + FVector(0, 0, 30),
												FootFloorLocation + FVector(0, 0, -25),
												4.f,
												TraceTypeQuery1,
												false,
												ActorsToIgnore,
#ifdef UE_BUILD_DEBUG
												EDrawDebugTrace::ForOneFrame,
#elif
			EDrawDebugTrace::None,
#endif
												HitResult,
												true);

		// Hit된 곳이 걸을 수 있음
		if (GetMovementComponent()->IsWalkable(HitResult))
		{
			FVector ImpactNormal;
			FVector ImpactPoint;

			ImpactPoint  = HitResult.ImpactPoint;
			ImpactNormal = HitResult.ImpactNormal;

			// 목표 위치 업데이트
			Current_Location_Target =
					(ImpactPoint + ImpactNormal * 10) - (FootFloorLocation + FVector::UpVector * 10);

			FRotator TargetRotationOffset;

			// 법선벡터에서 회전값을 가져옴 (Roll, Pitch)가 필요
			TargetRotationOffset = FRotator(-UKismetMathLibrary::DegAtan2(ImpactNormal.X, ImpactNormal.Z),
											0,
											UKismetMathLibrary::DegAtan2(ImpactNormal.Y, ImpactNormal.Z));

			// 현재 위치보다 목표 지점이 더 낮다?
			if (Current_Location_Offset.Z > Current_Location_Target.Z)
			{
				// 조금 더 빠르게 발 위치를 아래로 내려주자
				Current_Location_Offset = UKismetMathLibrary::VInterpTo(
																		Current_Location_Offset,
																		Current_Location_Target,
																		ComponentDeltaSeconds,
																		30.f);
			}
			// 현재 발 위치가 더 낮음
			else
			{
				// 조금 더 스무스하게 발 위치를 올려주자
				Current_Location_Offset = UKismetMathLibrary::VInterpTo(
																		Current_Location_Offset,
																		Current_Location_Target,
																		ComponentDeltaSeconds,
																		15.f);
			}

			// 회전값도 보간해서 업데이트
			Current_Rotation_Offset = UKismetMathLibrary::RInterpTo(Current_Rotation_Offset,
																	TargetRotationOffset,
																	ComponentDeltaSeconds,
																	30.f);
		}
	}
	else
	{
		Current_Location_Offset = FVector::ZeroVector;
		Current_Rotation_Offset = FRotator::ZeroRotator;
	}
}

void ULocomotionComponent::SetPelvisIKOffset(const FVector& FootOffset_L_Target, const FVector& FootOffset_R_Target)
{
	PelvisAlpha = (GetAnimCurveValue("Enable_Foot_IK_R") + GetAnimCurveValue("Enable_Foot_IK_L")) / 2;

	if (PelvisAlpha > 0.f)
	{
		const FVector PelvisTarget = FootOffset_L_Target.Z < FootOffset_R_Target.Z
										 ? FootOffset_L_Target
										 : FootOffset_R_Target;


		PelvisOffset = UKismetMathLibrary::VInterpTo(PelvisOffset,
													 PelvisTarget,
													 ComponentDeltaSeconds,
													 PelvisTarget.Z > PelvisOffset.Z ? 10.f : 15.f);
	}
	else
	{
		PelvisOffset = FVector::ZeroVector;
	}
}

void ULocomotionComponent::SetCharacterRotation(const FRotator& TargetRotation, const float InterpSpeedConstant,
												const float     InterpSpeed)
{
	GetCharacter()->SetActorRotation(CharacterLastTargetRotation);

	CharacterSmoothRotation = UKismetMathLibrary::RInterpTo_Constant(
																	 CharacterSmoothRotation,
																	 TargetRotation,
																	 ComponentDeltaSeconds,
																	 InterpSpeedConstant);
	CharacterLastTargetRotation = UKismetMathLibrary::RInterpTo(
																CharacterLastTargetRotation,
																CharacterSmoothRotation,
																ComponentDeltaSeconds,
																InterpSpeed);
}

void ULocomotionComponent::SetSelectedMovementData()
{
	// 오버레이 설정
	SelectedMovementData = *MovementData.Find(CurrentMovementType);
}

void ULocomotionComponent::SetCharacterMovementData() const
{
	UCharacterMovementComponent* MovementComponent = GetMovementComponent();

	MovementComponent->MaxWalkSpeed         = CurrentMovementData.WalkSpeed;
	MovementComponent->MaxWalkSpeedCrouched = CurrentMovementData.WalkSpeed;
	MovementComponent->MaxSwimSpeed         = CurrentMovementData.WalkSpeed;
	MovementComponent->MaxFlySpeed          = CurrentMovementData.WalkSpeed;

	MovementComponent->MaxAcceleration = CurrentMovementData.Acceleration;

	MovementComponent->BrakingDecelerationWalking  = CurrentMovementData.Deceleration;
	MovementComponent->BrakingDecelerationSwimming = CurrentMovementData.Deceleration;
	MovementComponent->BrakingDecelerationFlying   = CurrentMovementData.Deceleration;

	MovementComponent->GroundFriction                    = CurrentMovementData.Friction;
	MovementComponent->GetPhysicsVolume()->FluidFriction = CurrentMovementData.Friction;
}

void ULocomotionComponent::GetMovementDataByProfile(const EActionProfile TargetProfile, FMovementData& OutMovementData,
													EActionProfile&      OutProfile)
{
	FMovementByProfile MovementProfile = SelectedMovementData.MovementDataByOverlay.FindChecked(CurrentMovementOverlay).
															  MovementDataByStance.FindChecked(CurrentStance).
															  MovementDataByPose.FindChecked(CurrentPose).
															  MovementDataByModel.FindChecked(CurrentMovementModel);

	const FMovementData* ReturnData;

	if ((ReturnData = MovementProfile.MovementDataByProfile.Find(TargetProfile)) != nullptr)
	{
		OutProfile = TargetProfile;
	}
	else if ((ReturnData = MovementProfile.MovementDataByProfile.Find(EActionProfile::Low)) != nullptr)
	{
		OutProfile = EActionProfile::Low;
	}
	else if ((ReturnData = MovementProfile.MovementDataByProfile.Find(EActionProfile::Mid)) != nullptr)
	{
		OutProfile = EActionProfile::Mid;
	}
	else if ((ReturnData = MovementProfile.MovementDataByProfile.Find(EActionProfile::High)) != nullptr)
	{
		OutProfile = EActionProfile::High;
	}
	else
	{
		OutProfile = EActionProfile::None;
	}

	if (ReturnData)
	{
		OutMovementData = *ReturnData;
	}
	else
	{
		// 기본 값 설정
		OutMovementData.WalkSpeed    = 0.f;
		OutMovementData.Acceleration = 0.f;
		OutMovementData.Deceleration = 5000.f;
		OutMovementData.Friction     = 1.f;
		OutMovementData.RotationRate = 0.f;
	}
}

EActionProfile ULocomotionComponent::GetAllowedProfile()
{
	switch (ValidateGaitProfileForMovementData(DesiredGaitProfile))
	{
	case EActionProfile::None:
		break;
	case EActionProfile::Low:
		return EActionProfile::Low;
	case EActionProfile::Mid:
		return EActionProfile::Mid;
	case EActionProfile::High:
		return CanEnterHighProfile() ? EActionProfile::High : EActionProfile::Mid;
	}
	return EActionProfile::None;
}

EActionProfile ULocomotionComponent::GetActualProfile(const EActionProfile AllowedProfile)
{
	EActionProfile Low,     Mid,     High;
	FMovementData  LowData, MidData, HighData;

	GetMovementDataByProfile(EActionProfile::Low, LowData, Low);
	GetMovementDataByProfile(EActionProfile::Mid, MidData, Mid);
	GetMovementDataByProfile(EActionProfile::High, HighData, High);

	const float Local_LowSpeed  = LowData.WalkSpeed - 10;
	const float Local_MidSpeed  = MidData.WalkSpeed - 10;
	const float Local_HighSpeed = HighData.WalkSpeed - 10;

	if (MovementSpeed >= (Local_MidSpeed + (Local_HighSpeed - Local_MidSpeed) / 2))
	{
		return AllowedProfile;
	}

	if (MovementSpeed >= (Local_LowSpeed + (Local_MidSpeed - Local_LowSpeed) / 2))
	{
		return EActionProfile::Mid;
	}
	return EActionProfile::Low;
}

float ULocomotionComponent::GetAnimCurveValue(const FName& CurveName) const
{
	if (const UAnimInstance* AnimInstance = GetAnimInstance())
	{
		return AnimInstance->GetCurveValue(CurveName);
	}
	return 0.f;
}

float ULocomotionComponent::GetMovementSpeed() const
{
	return MovementSpeed;
}

FTurnInPlaceData ULocomotionComponent::GetTurnInPlaceData(const float TurnAngle)
{
	// TODO:
	return FTurnInPlaceData();
}

bool ULocomotionComponent::IsCrouching() const
{
	return CurrentStance == ELocomotionStances::Crouch;
}

bool ULocomotionComponent::CanEnterHighProfile() const
{
	if (!bHasMovementInput)
		return false;

	if (CurrentRotationMode == ERotationMode::VelocityDirection)
	{
		return MovementInputAlpha > 0.9f;
	}

	if (CurrentRotationMode == ERotationMode::LookingDirection)
	{
		return
				(MovementInputAlpha > 0.9f)
				&&
				(50 > abs(UKismetMathLibrary::NormalizedDeltaRotator(
																	 UKismetMathLibrary::MakeRotFromX(CurrentAcceleration),
																	 GetCharacter()->GetControlRotation()).Yaw));
	}
	return false;
}

bool ULocomotionComponent::CanTurnInPlace() const
{
	return
			(abs(AimingAngle) > TurnInPlaceThreshold)
			&&
			(AimYawRate < 50.f)
			&&
			(GetAnimCurveValue("EnableTransition") > 0.99)
			&&
			(CurrentRotationMode == ERotationMode::LookingDirection);
}

void ULocomotionComponent::HandleGround() const
{
	const float ControlYaw = GetCharacter()->GetControlRotation().Yaw;

	GetCharacter()->AddMovementInput(
									 UKismetMathLibrary::GetForwardVector(FRotator(0, ControlYaw, 0)),
									 MovementInput.X,
									 false);
	GetCharacter()->AddMovementInput(
									 UKismetMathLibrary::GetRightVector(FRotator(0, ControlYaw, 0)),
									 MovementInput.Y,
									 false);
}

void ULocomotionComponent::HandleSwimming() const
{
	const float ControlYaw   = GetCharacter()->GetControlRotation().Yaw;
	const float ControlPitch = GetCharacter()->GetControlRotation().Pitch;

	GetCharacter()->AddMovementInput(
									 UKismetMathLibrary::GetForwardVector(FRotator(ControlPitch, ControlYaw, 0)),
									 MovementInput.X,
									 false);
	GetCharacter()->AddMovementInput(
									 UKismetMathLibrary::GetRightVector(FRotator(ControlPitch, ControlYaw, 0)),
									 MovementInput.Y,
									 false);
}

void ULocomotionComponent::HandleWallClimbing()
{
	// Traversal Manager에서 관리
	ParkourComponent->TryMoveOnLedge(MovementInput);
}

void ULocomotionComponent::HandleFlying() const
{
	const float ControlYaw   = GetCharacter()->GetControlRotation().Yaw;
	const float ControlPitch = GetCharacter()->GetControlRotation().Pitch;

	GetCharacter()->AddMovementInput(
									 UKismetMathLibrary::GetForwardVector(FRotator(ControlPitch, ControlYaw, 0)),
									 MovementInput.X,
									 false);
	GetCharacter()->AddMovementInput(
									 UKismetMathLibrary::GetRightVector(FRotator(ControlPitch, ControlYaw, 0)),
									 MovementInput.Y,
									 false);
}

void ULocomotionComponent::HandleJumpAction(const bool bAction, const float ElapsedTime)
{
	bJumpActionPressed = bAction;

	if (GetCharacter()->IsPlayingRootMotion())
		return;

	if (CurrentMovementType == EMovementType::Ground)
	{
		if (bJumpActionPressed)
		{
			if (CurrentStance == ELocomotionStances::Neutral)
			{
				// 이걸 호출하기 전에 파쿠르 가능한지 확인 필요
				if (!ParkourComponent->TryTakeParkourAction())
				{
					GetCharacter()->Jump();
					UE_LOG(LogTemp, Log, TEXT("Jump Triggered"))
				}
			}
			else if (CurrentStance == ELocomotionStances::Crouch)
			{
				SetStance(ELocomotionStances::Neutral);
			}
		}
		else
		{
			if (CurrentStance == ELocomotionStances::Neutral)
			{
				GetCharacter()->StopJumping();
			}
		}
	}
	else if (CurrentMovementType == EMovementType::LedgeOrHang)
	{
		if (bJumpActionPressed)
		{
			if (!ParkourComponent->TryHop())
			{
				GetCharacter()->LaunchCharacter(
												FVector(CurrentInputDirection.X, CurrentInputDirection.Y, 1) * 300.f,
												true,
												true);
			}
		}
	}
}

void ULocomotionComponent::HandleCrouchAction(const bool bAction, const float ElapsedTime)
{
	if (CurrentMovementType == EMovementType::Ground)
	{
		if (CrouchAction_HandleType == EInputHandleType::Toggle && !bAction)
		{
			return;
		}

		SetStance(
				  CurrentStance == ELocomotionStances::Crouch ? ELocomotionStances::Neutral : ELocomotionStances::Crouch);
	}
}

void ULocomotionComponent::HandleLowProfileAction(const bool bAction, const float ElapsedTime)
{
	if (CurrentMovementType == EMovementType::Ground)
	{
		if (WalkAction_HandleType == EInputHandleType::Toggle && !bAction)
		{
			return;
		}
		SetDesiredGait(
					   DesiredGaitProfile == EActionProfile::Low ? EActionProfile::Mid : EActionProfile::Low);
	}
}

void ULocomotionComponent::HandleHighProfileAction(const bool bAction, const float ElapsedTime)
{
	bHighProfileActionPressed = bAction;

	if (CurrentMovementType == EMovementType::Ground || CurrentMovementType == EMovementType::Swim ||
		CurrentMovementType == EMovementType::Fly)
	{
		if (SprintAction_HandleType == EInputHandleType::Toggle && !bAction)
		{
			return;
		}

		if (CurrentMovementType == EMovementType::Ground || CurrentMovementType == EMovementType::Fly)
		{
			SetDesiredGait(
						   (DesiredGaitProfile == EActionProfile::Mid && bAction)
							   ? EActionProfile::High
							   : EActionProfile::Mid);
		}
		else if (CurrentMovementType == EMovementType::Swim)
		{
			SetDesiredGait(
						   (DesiredGaitProfile == EActionProfile::Low && bAction)
							   ? EActionProfile::Mid
							   : EActionProfile::Low);
		}
	}
}

void ULocomotionComponent::HandleSecondaryProfileAction(const EInputResult InputResult, bool bAction,
														const float        ElapsedTime)
{
	if (CurrentMovementType == EMovementType::Ground ||
		CurrentMovementType == EMovementType::Swim ||
		CurrentMovementType == EMovementType::Fly)
	{
		// Third Person
		switch (InputResult)
		{
		case EInputResult::Started:
			SetPose(EPose::Aiming);
			break;
		case EInputResult::Tapped:
		case EInputResult::HeldReleased:
			SetPose(EPose::Neutral);
			break;
		case EInputResult::Held:
			break;
		}
	}
}

void ULocomotionComponent::HandleMovementInput(const FVector2D Axis2D)
{
	MovementInput = Axis2D;

	switch (CurrentMovementType)
	{
	case EMovementType::Ground:
	case EMovementType::Slide:
		HandleGround();
		break;
	case EMovementType::Swim:
		HandleSwimming();
		break;
	case EMovementType::Fly:
		HandleFlying();
		break;
	case EMovementType::LedgeOrHang:
		HandleWallClimbing();
		break;
	case EMovementType::Parkour:
		HandleWallClimbing();
	case EMovementType::Cover:
	case EMovementType::Ride:
		break;
	}
}

void ULocomotionComponent::HandleLookInput(const FVector2D Axis2D) const
{
	ACharacter* TargetCharacter = GetCharacter();

	const FVector2D InterpolatedAxis = Axis2D * (LookInputSensitivity * ComponentDeltaSeconds);

	TargetCharacter->AddControllerPitchInput(InterpolatedAxis.X);
	TargetCharacter->AddControllerYawInput(InterpolatedAxis.Y);
}

EOverlayStates ULocomotionComponent::ValidateOverlayForMovementData(const EOverlayStates TargetOverlay)
{
	const FMovementByOverlay* MovementByOverlay = MovementData.Find(CurrentMovementType);

	if (!MovementByOverlay->MovementDataByOverlay.IsEmpty())
	{
		TArray<EOverlayStates> Keys;
		MovementByOverlay->MovementDataByOverlay.GetKeys(Keys);

		return MovementByOverlay->MovementDataByOverlay.Contains(TargetOverlay)
				   ? TargetOverlay
				   : Keys[0];
	}

	return TargetOverlay;
}

ELocomotionStances ULocomotionComponent::ValidateStanceForMovementData(const ELocomotionStances TargetStance)
{
	const FMovementByOverlay* MovementByOverlay = MovementData.Find(CurrentMovementType);

	if (!MovementByOverlay->MovementDataByOverlay.IsEmpty())
	{
		const FMovementByStance* MovementByStance = MovementByOverlay->MovementDataByOverlay.Find(
		 CurrentMovementOverlay);

		TArray<ELocomotionStances> Keys;
		MovementByStance->MovementDataByStance.GetKeys(Keys);

		return MovementByStance->MovementDataByStance.Contains(TargetStance)
				   ? TargetStance
				   : !MovementByStance->MovementDataByStance.IsEmpty()
				   ? Keys[0]
				   : ELocomotionStances::None;
	}

	return ELocomotionStances::None;
}

EMovementModel ULocomotionComponent::ValidateMovementModelForMovementData(const EMovementModel TargetMovementModel)
{
	const FMovementByModel* MovementByModel = MovementData.FindChecked(CurrentMovementType).
														   MovementDataByOverlay.FindChecked(CurrentMovementOverlay).
														   MovementDataByStance.FindChecked(CurrentStance).
														   MovementDataByPose.Find(CurrentPose);
	TArray<EMovementModel> Keys;
	MovementByModel->MovementDataByModel.GetKeys(Keys);
	if (MovementByModel)
	{
		return MovementByModel->MovementDataByModel.Contains(TargetMovementModel)
				   ? TargetMovementModel
				   : (!MovementByModel->MovementDataByModel.IsEmpty())
				   ? Keys[0]
				   : EMovementModel::None;
	}
	return EMovementModel::None;
}

EActionProfile ULocomotionComponent::ValidateGaitProfileForMovementData(const EActionProfile TargetProfile)
{
	const FMovementByProfile* MovementByProfile = MovementData.FindChecked(CurrentMovementType).
															   MovementDataByOverlay.FindChecked(CurrentMovementOverlay)
															   .
															   MovementDataByStance.FindChecked(CurrentStance).
															   MovementDataByPose.FindChecked(CurrentPose).
															   MovementDataByModel.Find(CurrentMovementModel);
	TArray<EActionProfile> Keys;
	MovementByProfile->MovementDataByProfile.GetKeys(Keys);

	if (MovementByProfile)
	{
		if (MovementByProfile->MovementDataByProfile.Contains(TargetProfile))
		{
			return TargetProfile;
		}
		if (!MovementByProfile->MovementDataByProfile.IsEmpty())
		{
			for (const EActionProfile Key : Keys)
			{
				if (Key == TargetProfile)
				{
					return Key;
				}
			}
		}
	}

	return EActionProfile::None;
}

EPose ULocomotionComponent::ValidatePoseForMovementData(const EPose TargetPose)
{
	if (const FMovementByPose* MovementByPose = MovementData.FindChecked(CurrentMovementType).
															 MovementDataByOverlay.FindChecked(CurrentMovementOverlay).
															 MovementDataByStance.Find(CurrentStance))
	{
		if (MovementByPose->MovementDataByPose.Contains(CurrentPose))
		{
			return TargetPose;
		}
		TArray<EPose> Keys;
		MovementByPose->MovementDataByPose.GetKeys(Keys);
		if (!MovementByPose->MovementDataByPose.IsEmpty())
		{
			return Keys[0];
		}
	}
	return EPose::None;
}

float ULocomotionComponent::SelectVelocitySpeed() const
{
	switch (CurrentMovementMode)
	{
	case MOVE_None:
	case MOVE_Walking:
	case MOVE_NavWalking:
	case MOVE_Falling:
	case MOVE_Custom:
	case MOVE_MAX:
		return GetCharacter()->GetVelocity().Size2D();
	case MOVE_Swimming:
	case MOVE_Flying:
		return GetCharacter()->GetVelocity().Size();
	default: ;
	}
	return 0;
}


bool ULocomotionComponent::CanUpdateMovingRotation() const
{
	return ((bIsMoving && bHasMovementInput && MovementSpeed > 50.f)
		|| MovementSpeed > 150.f) && !GetCharacter()->HasAnyRootMotion();
}


void ULocomotionComponent::OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode,
												 uint8       PreviousCustomMode)
{
	Super::OnMovementModeChanged(Character, PrevMovementMode, PreviousCustomMode);

	PreviousMovementMode = PrevMovementMode;
	CurrentMovementMode  = GetMovementComponent()->MovementMode;

	if (MOVE_Walking == PreviousMovementMode && CurrentMovementMode == MOVE_Falling)
	{
		if (CurrentMovementType == EMovementType::Ground)
		{
			CurrentCharacterState = GetCharacter()->GetVelocity().Z <= 0
										? ECharacterState::Falling
										: ECharacterState::InAir;

			LastAirRotation = bIsMoving
								  ? UKismetMathLibrary::MakeRotFromX(GetCharacter()->GetVelocity())
								  : GetCharacter()->GetActorRotation();
		}
	}
	else if (MOVE_Walking == CurrentMovementMode && MOVE_Falling == PreviousMovementMode)
	{
		CurrentCharacterState = ECharacterState::Stable;

		if (CurrentStance == ELocomotionStances::Crouch)
		{
			if (!GetMovementComponent()->IsCrouching())
			{
				GetCharacter()->Crouch();
			}
		}
	}
}

void ULocomotionComponent::OnCharacterMovementUpdated(float DeltaSeconds, FVector OldLocation, FVector OldVelocity)
{
	Super::OnCharacterMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	ComponentDeltaSeconds = DeltaSeconds;

	// Velocity, Acceleration, Input ... 업데이트
	UpdateMovementData();

	// Direction, Aim yaw ...  업데이트
	UpdateRotationData();

	//TODO: Turn In Place

	// Air 상태 데이터 업데이트
	UpdateFallData();

	// 업데이트 된 데이터 캐릭터에게 반영
	UpdateCharacterRotation();
	UpdateCharacterMovement();
	UpdateFootIKValues();

	// 입력 방향 디버깅
	DrawDebugLine(
				  GetWorld(),
				  GetCharacter()->GetActorLocation(),
				  GetCharacter()->GetActorLocation() + CurrentInputDirection * 100.f,
				  FColor::Orange,
				  false,
				  0.01f,
				  0,
				  1.5f);

	// 캐릭터 방향 디버깅
	DrawDebugLine(
				  GetWorld(),
				  GetCharacter()->GetActorLocation(),
				  GetCharacter()->GetActorLocation() + GetCharacter()->GetActorForwardVector() * 80.f,
				  FColor::Black,
				  false,
				  0.01f,
				  0,
				  1.5f);
}

void ULocomotionComponent::OnCharacterLanded(const FHitResult& Hit)
{
	Super::OnCharacterLanded(Hit);

	GetMovementComponent()->NavAgentProps.bCanJump = false;

	GetWorld()->GetTimerManager().SetTimer(JumpActivatorTimerHandle,
										   this,
										   &ThisClass::ActivateJumpAfterLand,
										   0.3f,
										   false);

	for (const ULandAction* LandDataAsset : LandDataAssets)
	{
		if (LandDataAsset->ApexDistanceSetup.Contains(ApexDistance) && LandDataAsset->SpeedSetup.
			Contains(MovementSpeed))
		{
			GetCharacter()->PlayAnimMontage(
											LandDataAsset->LandAnims[FMath::RandRange(0,
												LandDataAsset->LandAnims.Num() - 1)]);
		}
	}
}


void ULocomotionComponent::OnMontageBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	Super::OnMontageBlendOut(Montage, bInterrupted);
}

void ULocomotionComponent::OnReachedApex()
{
	Super::OnReachedApex();

	ApexDistance = GroundDistance;
}

void ULocomotionComponent::OnMovementTypeUpdated(const EMovementType TargetMovementType,
												 const EMovementType Previous)
{
	if (TargetMovementType == EMovementType::Ground && Previous != EMovementType::Parkour)
	{
		GetMovementComponent()->SetMovementMode(MOVE_Walking);
		CurrentCharacterState = ECharacterState::Stable;
	}
}

void ULocomotionComponent::ActivateJumpAfterLand()
{
	GetMovementComponent()->NavAgentProps.bCanJump = true;
	GetWorld()->GetTimerManager().ClearTimer(JumpActivatorTimerHandle);
	JumpActivatorTimerHandle.Invalidate();
}

bool ULocomotionComponent::CircularDetection(const FVector&      Position, const FVector& Direction, const int Iteration,
											 const float         Radius, const float SphereRadius, const float Height,
											 const float         Degrees, const float Offset,
											 const FLinearColor& TraceColor, TArray<FHitResult>& AvailablePoints,
											 TArray<FHitResult>& AllPoints) const
{
	TArray<FHitResult> AllResults;
	TArray<FHitResult> Hits;

	for (int i = 0; i <= Iteration; ++i)
	{
		FHitResult      Local_HitResult;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetOwner());

		int Angle = Degrees / Iteration; // 13도 (총 20개의 구체로 확인)

		FVector CircularLocation = UKismetMathLibrary::Quat_RotateVector(
																		 UKismetMathLibrary::MakeRotFromX(Direction).
																		 Quaternion(),
																		 UKismetMathLibrary::Quat_RotateVector(
																		  // 로테이터는 Z축이 뒤를 보게설정 -180
																		  FRotator(0, Degrees * -0.5 + Offset, 0).
																		  Quaternion(),
																		  // (Cos(Angle), Sin(Angle)) XY좌표로 설정해서 360도를 일정한 간격으로 조사
																		  FVector(
																				  FMath::Cos(FMath::DegreesToRadians(Angle)
																				   * i) *
																				  Radius,
																				  FMath::Sin(FMath::DegreesToRadians(Angle)
																				   * i) *
																				  Radius,
																				  0)));

		UKismetSystemLibrary::SphereTraceSingle(
												GetWorld(),
												Position + CircularLocation + (FVector::UpVector * Height / 2),
												Position + CircularLocation - (FVector::UpVector * Height / 2),
												SphereRadius,
												TraceTypeQuery1,
												false,
												ActorsToIgnore,
												EDrawDebugTrace::ForDuration,
												Local_HitResult,
												true,
												TraceColor);

		AllResults.Add(Local_HitResult);

		if (Local_HitResult.IsValidBlockingHit())
		{
			Hits.Add(Local_HitResult);
		}
	}

	AvailablePoints = Hits;
	AllPoints       = AllResults;
	return !Hits.IsEmpty();
}
