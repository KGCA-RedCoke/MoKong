// copyright Jacob Lim


#include "Component/ParkourComponent.h"

#include "MotionWarpingComponent.h"
#include "Component/LocomotionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


void UParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	check(GetOwner())

	// 컴포넌트 레퍼런스 가져오기
	MotionWarpingComponent = Cast<
		UMotionWarpingComponent>(GetOwner()->GetComponentByClass(UMotionWarpingComponent::StaticClass()));

	LocomotionComponent = Cast<ULocomotionComponent>(
													 GetCharacter()->
													 GetComponentByClass(ULocomotionComponent::StaticClass()));
	SplinePath = Cast<USplineComponent>(
										GetOwner()->AddComponentByClass(USplineComponent::StaticClass(),
																		false,
																		FTransform::Identity,
																		false));

	check(MotionWarpingComponent)
	check(LocomotionComponent)
	check(SplinePath)
}

void UParkourComponent::OnCharacterMovementUpdated(float DeltaSeconds, FVector OldLocation, FVector OldVelocity)
{
	Super::OnCharacterMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	CurrentInputDirection = LocomotionComponent->CurrentInputDirection;
	LastInputDirection    = LocomotionComponent->LastInputDirection;

	if (LocomotionComponent->CurrentMovementType == EMovementType::Ground ||
		LocomotionComponent->CurrentMovementType == EMovementType::Parkour)
	{
		if (CurrentParkourState == EParkourState::None)
		{
			// 점프를 이미 시작했는데 앞에 벽이 있으면 파쿠르 액션을 다시 검사하고 실행해도 좋음
			if (bJumpActionTriggered)
			{
				TryTakeParkourAction();
			}
		}
		else if (CurrentParkourState == EParkourState::StepUp || CurrentParkourState == EParkourState::WallRun)
		{
			// TODO: Wall Running
			// TryJumpToWall();
		}
		else if (CurrentParkourState == EParkourState::Vault || CurrentParkourState == EParkourState::Mantle)
		{
			SetRotationValues(UKismetMathLibrary::MakeRotFromX(GetCharacter()->GetActorForwardVector()));
		}
	}
	else if (LocomotionComponent->CurrentMovementType == EMovementType::LedgeOrHang)
	{
		if (CurrentParkourState == EParkourState::Ledge || CurrentParkourState == EParkourState::Hang ||
			CurrentParkourState == EParkourState::Mantle)
		{
			CurrentParkourState = CheckLedgeHasSurface() ? EParkourState::Ledge : EParkourState::Hang;
		}
	}
}

void UParkourComponent::OnMontageBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (CachedMontage == Montage)
	{
		PreviousParkourState = EParkourState::None;

		// 위로 올라가는 모션
		if (CurrentParkourState == EParkourState::StepUp)
		{
			SetParkourState(EParkourState::None);
			WallRunState = EWallRunState::None;

			// // TODO: Reset Motion Warping
			ResetMotionWarping();

			LocomotionComponent->SetMovementType(EMovementType::Ground);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			GetMovementComponent()->SetMovementMode(MOVE_Walking);

			LocomotionComponent->CurrentCharacterState = ECharacterState::Stable;

			GetCharacter()->GetController()->SetIgnoreMoveInput(false);
		}
		// 매달리는 모션
		else if (CurrentParkourState == EParkourState::Hang || CurrentParkourState == EParkourState::Ledge ||
			CurrentParkourState == EParkourState::Mantle)
		{
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

			GetCharacter()->GetController()->SetIgnoreMoveInput(false);
		}
		// 넘어가는 모션
		else if (CurrentParkourState == EParkourState::Vault)
		{
			SetParkourState(EParkourState::None);
			WallRunState = EWallRunState::None;

			ResetMotionWarping();

			LocomotionComponent->SetMovementType(EMovementType::Ground);

			GetCharacter()->GetController()->SetIgnoreMoveInput(false);

			FFindFloorResult FloorResult;
			GetMovementComponent()->ComputeFloorDist(GetCapsuleComponent()->GetComponentLocation(),
													 1000.f,
													 1000.f,
													 FloorResult,
													 5.f);

			GetMovementComponent()->SetMovementMode(MOVE_Falling);
			LocomotionComponent->CurrentCharacterState = FloorResult.bBlockingHit && FloorResult.FloorDist < 80.f
															 ? ECharacterState::Stable
															 : ECharacterState::Falling;
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

			GetCharacter()->GetController()->SetIgnoreMoveInput(false);
		}
		else if (CurrentParkourState == EParkourState::Dodge)
		{
			SetParkourState(EParkourState::None);

			GetCharacter()->GetController()->SetIgnoreMoveInput(false);
		}
	}
	else if (CurrentParkourState == EParkourState::None)
	{
		SetRotationValues(FRotator(0, GetCharacter()->GetActorRotation().Yaw, 0));
	}
}

void UParkourComponent::HandleRollAndDodgeAction(const bool bPressed, EDodgeType DodgeType, EMovementDirection Direction,
												 float MoveDistance, UAnimMontage*& MontageToPlay, FName& SectionToPlay)
{
	if (bPressed &&
		CurrentParkourState == EParkourState::None &&
		LocomotionComponent->CurrentCharacterState == ECharacterState::Stable)
	{
		const ERotationMode RotationMode  = LocomotionComponent->CurrentRotationMode;
		const FVector       MoveDirection = (CurrentInputDirection.IsNearlyZero()
												 ? GetCharacter()->
												 GetActorForwardVector()
												 : CurrentInputDirection);

		const FVector StartLocation = GetCharacter()->GetActorLocation() + 100 * MoveDirection;

		FRotator Rotation = CurrentInputDirection.IsNearlyZero()
								? GetCharacter()->GetActorRotation()
								: UKismetMathLibrary::MakeRotFromX(CurrentInputDirection);

		SetParkourState(EParkourState::Dodge);
		switch (DodgeType)
		{
		case EDodgeType::Dodge:
			CachedMontage = DodgeMontage;
			break;
		case EDodgeType::PerfectDodge:
			CachedMontage = PerfectDodgeMontage;
			break;
		case EDodgeType::Roll:
			CachedMontage = RollMontage;
			break;
		case EDodgeType::JXSQ:
			CachedMontage = JXSQDodgeMontage;
			break;
		}
		MontageToPlay = CachedMontage;

		SectionToPlay = "Forward";

		if (RotationMode == ERotationMode::LookingDirection)
		{
			MotionWarpingComponent->RemoveWarpTarget("Rotation");

			switch (Direction)
			{
			case EMovementDirection::Forward:
				SectionToPlay = "Forward";
				MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(
																		   "Rotation",
																		   FTransform(Rotation, StartLocation));
				break;
			case EMovementDirection::Backward:
				Rotation.Yaw += 180.f;
				MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(
																		   "Rotation",
																		   FTransform(Rotation, StartLocation));
				SectionToPlay = "Backward";
				break;
			case EMovementDirection::Left:
				SectionToPlay = "Left";
				break;
			case EMovementDirection::Right:
				SectionToPlay = "Right";
				break;
			}

			MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(
																	  "Move",
																	  GetCharacter()->GetActorLocation() +
																	  MoveDistance * MoveDirection);
		}
		else
		{


			MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(
																	   "Rotation",
																	   FTransform(Rotation, StartLocation));

			MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(
																	   "Move",
																	   FTransform(Rotation,
																					  GetCharacter()->GetActorLocation() +
																					  MoveDistance * MoveDirection));

			LocomotionComponent->SetCharacterRotation(
													  FRotator(0, Rotation.Yaw, 0),
													  0.f,
													  0.f);
			SetRotationValues(FRotator(0, Rotation.Yaw, 0));
		}

		GetCharacter()->GetController()->SetIgnoreMoveInput(true);
	}
}

bool UParkourComponent::CanTraceWall(const FVector& Direction) const
{
	return !CurrentInputDirection.IsZero() &&
	(UKismetMathLibrary::DegAcos(
								 UKismetMathLibrary::MakeRotFromX(Direction).Quaternion().GetForwardVector().Dot(
								  GetCharacter()->GetActorForwardVector())) <= WallTrace_AngleThreshold);
}

bool UParkourComponent::TraceWall(const FVector&  Position, const FVector& Direction,
								  float           Height, int              Iteration, EParkourWallHitResult& WallHitResult,
								  FWallHitResult& FinalResult) const
{
	bool bFirstHitAssigned = false; // 첫번째 Hit 결과는 저장만하고 계산(비교하는 계산이기 때문)은 필요없음

	int HitCounter = 0; // Hit된 갯수가 Iteration과 일치하면 종료

	FHitResult FirstHitResult; // 가장 처음 벽에 Hit된 결과 (아마 벽 제일 하단 부분일 것임)
	FHitResult BoxTraceResult; // 캐릭터 위치에서 아래부터 위로 벽을 향해 가속 방향으로 BoxTrace결과

	TArray<AActor*> ActorsToIgnore; // 자기 자신에 대한 Trace결과는 무시
	ActorsToIgnore.Add(GetOwner());

	FVector FirstPosition; // 벽에 손을 짚어야 할 부분
	FVector LastForwardNormal; // 손을 집어야 할 부분의 법선

	float CachedHeight     = 0.f; // 벽 높이 후보
	float CachedVaultDepth = 0.f; // 벽 두께 후보
	float CapsuleRadius, CapsuleHalfHeight; // 캐릭터 캡슐 지름, 높이

	GetCapsuleSize(true, CapsuleRadius, CapsuleHalfHeight);

	// 캐릭터 아래서부터 위로 Iteration만큼 BoxTrace를 실행
	for (int i = 0; i < WallTrace_Iteration; ++i)
	{
		int Index = i;

		// 이전에 있던 Trace결과는 제거
		BoxTraceResult.Init();

		// 시작 위치는 캐릭터 위치에서 살짝 위(기본값 : 3.5)로 + Iteration마다 더 올림 그리고 캐릭터 가속 방향으로 틀어줌
		FVector TraceStart =
				Position - FVector::UpVector * (CapsuleHalfHeight - Height / (Iteration * 2))
				+ UKismetMathLibrary::MakeRotFromX(Direction).Quaternion().GetForwardVector() * CapsuleRadius / 2
				+ FVector::UpVector * (Height / Iteration * Index);
		// 시작 위치에서 가속 방향으로 WallTraceDistance(기본값 : 300)만큼 앞으로 뻗어줌
		FVector TraceEnd =
				TraceStart + UKismetMathLibrary::MakeRotFromX(Direction).Quaternion().GetForwardVector() *
				WallTrace_Distance;

		// 디버그 드로
		if (bEnableDebug)
		{
			DrawDebugBox(GetWorld(), TraceStart, FVector::OneVector * 3, FColor::Orange, false, 3.f, 0, 3.f);
			DrawDebugBox(GetWorld(), TraceEnd, FVector::OneVector * 3, FColor::Orange, false, 3.f, 0, 3.f);
		}

		UKismetSystemLibrary::BoxTraceSingle(
											 GetWorld(),
											 TraceStart,
											 TraceEnd,
											 FVector(0.1,
													 WallTrace_Width,
													 Height / (Iteration * 2)),
											 UKismetMathLibrary::MakeRotFromX(Direction),
											 TraceTypeQuery1,
											 false,
											 ActorsToIgnore,
											 EDrawDebugTrace::None,
											 BoxTraceResult,
											 true
											);

		// 오버랩X 히트O
		if (BoxTraceResult.IsValidBlockingHit())
		{
			HitCounter++;

			// 앞에 Hit된 결과(벽)은 아마 수직이거나 캐릭터가 걸어다니지 못해야함
			if (!GetMovementComponent()->IsWalkable(BoxTraceResult))
			{
				// 이후 Hit결과 판별
				if (bFirstHitAssigned)
				{
					// 첫 벽과 거의 평행을 이루고있나? 벽이 수직이라면 Acos의 값은 대부분 0일 것
					if (UKismetMathLibrary::DegAcos(BoxTraceResult.Normal.Dot(FirstHitResult.Normal)) <= 30.f)
					{
						// Hit된 곳의 약간 앞쪽에서 
						FVector BoxHit_15O_OffFront = UKismetMathLibrary::ProjectPointOnToPlane(
						 BoxTraceResult.Location,
						 BoxTraceResult.ImpactPoint,
						 BoxTraceResult.Normal) - BoxTraceResult.Normal * 15.f;

						FHitResult HitResult_WallTop;

						UKismetSystemLibrary::BoxTraceSingle(
															 GetWorld(),
															 BoxHit_15O_OffFront + (Height / (Iteration * 2) *
																 FVector::UpVector), // 이 위치에서 5만큼 위로
															 BoxHit_15O_OffFront + (Height / (Iteration * 2) *
																 FVector::DownVector), // 이 위치에서 5만큼 아래로
															 FVector(15.f, 15.f, 0.f),
															 UKismetMathLibrary::MakeRotFromX(BoxTraceResult.Normal),
															 TraceTypeQuery1,
															 false,
															 ActorsToIgnore,
															 EDrawDebugTrace::None,
															 HitResult_WallTop,
															 true);

						// Hit결과 검증
						if (HitResult_WallTop.IsValidBlockingHit())
						{
							// 여기는 플레이어가 걸을 수 있어야 함 (그래야 짚을수 있는 후보군에 들어갈 수 있음)
							if (GetMovementComponent()->IsWalkable(HitResult_WallTop))
							{
								// 벽의 표면에 점을 투영
								FVector PointToWall = UKismetMathLibrary::ProjectPointOnToPlane(
								 BoxTraceResult.Location,
								 BoxTraceResult.ImpactPoint,
								 BoxTraceResult.Normal);
								// 벽 상단에서 살짝 위
								const float SlightlyUpped_TopHeight = (HitResult_WallTop.ImpactPoint + FVector(
								 0,
								 0,
								 2.f)).Z;

								// 이 부분이 잡을 수 있는 지점의 후보가 됨
								FirstPosition = FVector(PointToWall.X, PointToWall.Y, SlightlyUpped_TopHeight);

								// 잡을 수 있는 부분의 법선
								LastForwardNormal = BoxTraceResult.Normal;


								// 벽의 높이 저장
								CachedHeight =
										FMath::TruncToInt(abs(
															  FirstPosition.Z - (GetCharacter()->GetActorLocation().Z -
																  CapsuleHalfHeight) -
															  2));
								// (FirstPosition -
								// 	FVector(0, 0, (GetCharacter()->GetActorLocation() - CapsuleHalfHeight).Z)
								// ).Z - 2));

								// 잡을 수 있는 지점에서 (캡슐 반 높이 위 & 조금 더 앞으로) CapsuleTrace 이건 캐릭터가 위로 올라갈 수 있는지 판별
								FHitResult CapsuleHitResult;
								UKismetSystemLibrary::CapsuleTraceSingle(
																		 GetWorld(),
																		 (FirstPosition + (-LastForwardNormal *
																			 CapsuleRadius / 1.5)) + FVector(
																		  0,
																		  0,
																		  CapsuleHalfHeight),
																		 (FirstPosition + (-LastForwardNormal *
																			 CapsuleRadius / 1.5)) + FVector(
																		  0,
																		  0,
																		  CapsuleHalfHeight),
																		 CapsuleRadius * 0.9f,
																		 CapsuleHalfHeight * 0.9f,
																		 TraceTypeQuery1,
																		 false,
																		 ActorsToIgnore,
																		 EDrawDebugTrace::ForOneFrame,
																		 CapsuleHitResult,
																		 true);

								if (!CapsuleHitResult.bBlockingHit)
								{
									if (CachedHeight >= 30.f)
									{
										// 너무 높음
										if (CachedHeight >= 250.f)
										{
											// Wall 구조체 채우기 (필요한 정보는 첫번째 Hit위치, 벽 법선)
											FinalResult.bLandable         = false;
											FinalResult.bVaultable        = false;
											FinalResult.FirstPosition     = FirstHitResult.ImpactPoint;
											FinalResult.DepthPosition     = FVector::ZeroVector;
											FinalResult.FloorPosition     = FVector::ZeroVector;
											FinalResult.WallNormal        = FirstHitResult.Normal;
											FinalResult.Height            = 0;
											FinalResult.Depth             = 0;
											FinalResult.BottomEmptyHeight = 0;

											WallHitResult = EParkourWallHitResult::WallRun;
											return true;
										}

										if (CachedHeight >= 200.f && BoxTraceResult.Component->
											GetCollisionResponseToChannel(
																		  ECC_GameTraceChannel1) != ECR_Block)
										{
											FinalResult.bLandable         = false;
											FinalResult.bVaultable        = false;
											FinalResult.FirstPosition     = FirstPosition;
											FinalResult.DepthPosition     = FirstPosition + LastForwardNormal * 35.f;
											FinalResult.FloorPosition     = FirstPosition + LastForwardNormal * 35.f;
											FinalResult.WallNormal        = LastForwardNormal;
											FinalResult.Height            = CachedHeight;
											FinalResult.Depth             = 0;
											FinalResult.BottomEmptyHeight = 0;

											WallHitResult = EParkourWallHitResult::Mantle;
											return true;
										}

										// 이 오브젝트가 Vault가능하도록 TraceChannel 설정되었나?
										if (BoxTraceResult.Component->GetCollisionResponseToChannel(
										 ECC_GameTraceChannel1) != ECR_Block)
										{
											FinalResult.bLandable         = false;
											FinalResult.bVaultable        = false;
											FinalResult.FirstPosition     = FirstPosition;
											FinalResult.DepthPosition     = FirstPosition - LastForwardNormal * 35.f;
											FinalResult.FloorPosition     = FirstPosition - LastForwardNormal * 35.f;
											FinalResult.WallNormal        = LastForwardNormal;
											FinalResult.Height            = CachedHeight;
											FinalResult.Depth             = 0;
											FinalResult.BottomEmptyHeight = 0;

											WallHitResult = EParkourWallHitResult::Step;
											return true;
										}

										// 적당한 높이의 Vault TraceChannel을 가진 오브젝트인가?
										if (CachedHeight >= 60.f)
										{
											// 그럼 여기선 Vault로 얼마나 멀리 갈 수 있는지 앞 방향으로 Trace를 진행
											for (int VaultIndex = 0; VaultIndex <= (Vault_MaxDepth / 10) + 1; ++
												 VaultIndex)
											{
												FHitResult SphereTraceResult;
												UKismetSystemLibrary::SphereTraceSingle(
												 GetWorld(),
												 (FirstPosition - (LastForwardNormal * VaultIndex * 10)) + FVector(
												  0,
												  0,
												  10),
												 (FirstPosition - (LastForwardNormal * VaultIndex * 10)) + FVector(
												  0,
												  0,
												  -20),
												 3.f,
												 TraceTypeQuery1,
												 false,
												 ActorsToIgnore,
												 EDrawDebugTrace::None,
												 SphereTraceResult,
												 true);

												if (SphereTraceResult.IsValidBlockingHit())
												{
													SphereTraceResult.Reset();

													// DrawDebugBox(
													// 	GetWorld(), (SphereTraceResult.TraceStart - FVector(0, 0, 16)) -
													// 	LastForwardNormal * 10, FVector::OneVector * 3.f, FColor::Cyan,
													// 	false, 10.f, 0.f, 3.f);
													// DrawDebugBox(
													// 	GetWorld(), (SphereTraceResult.TraceStart - FVector(0, 0, 16)) +
													// 	LastForwardNormal * 10, FVector::OneVector * 3.f, FColor::Cyan,
													// 	false, 10.f, 0.f, 3.f);

													// 벽의 앞 부분으로 계속해서 Trace를 진행
													UKismetSystemLibrary::SphereTraceSingle(
													 GetWorld(),
													 (SphereTraceResult.TraceStart - FVector(0, 0, 16)) -
													 LastForwardNormal * 10,
													 (SphereTraceResult.TraceStart - FVector(0, 0, 16)) +
													 LastForwardNormal * 10,
													 3.f,
													 TraceTypeQuery1,
													 false,
													 ActorsToIgnore,
													 EDrawDebugTrace::ForDuration,
													 SphereTraceResult,
													 true,
													 FLinearColor::Blue,
													 FLinearColor::Green,
													 10.f);

													if (SphereTraceResult.IsValidBlockingHit() &&
														!GetMovementComponent()->IsWalkable(SphereTraceResult))
													{
														// 벽 깊이 끝자락
														FVector WallEndPoint = SphereTraceResult.ImpactPoint + FVector(
														 0,
														 0,
														 5);

														// 넘어갈 벽 깊이 저장
														CachedVaultDepth = round(
																				 FVector::Distance(FirstPosition,
																						 WallEndPoint
																					 ));

														FFindFloorResult FloorResult;
														// 바닥까지의 거리 반환
														GetMovementComponent()->ComputeFloorDist(
														 SphereTraceResult.ImpactPoint + FVector(
														  0,
														  0,
														  5) + SphereTraceResult.Normal * 100.f,
														 300.f,
														 300.f,
														 FloorResult,
														 2.f);

														// 도착 예상 지점에 바닥이 없으면 Vault 불가능
														if (FloorResult.HitResult.bBlockingHit)
														{
															if (bEnableDebug)
																DrawDebugCapsule(
																				 GetWorld(),
																				 FloorResult.HitResult.ImpactPoint +
																				 FVector(
																				  0,
																				  0,
																				  90),
																				 CapsuleHalfHeight * 0.9f,
																				 CapsuleRadius * 0.9f,
																				 FQuat::Identity,
																				 FColor::Black,
																				 false,
																				 3.f,
																				 0,
																				 1.f
																				);

															CapsuleHitResult.Init();

															UKismetSystemLibrary::CapsuleTraceSingle(
															 GetWorld(),
															 FloorResult.HitResult.ImpactPoint + FVector(0, 0, 90),
															 FloorResult.HitResult.ImpactPoint + FVector(0, 0, 90),
															 CapsuleRadius * 0.9f,
															 CapsuleHalfHeight * 0.9f,
															 TraceTypeQuery1,
															 false,
															 ActorsToIgnore,
															 EDrawDebugTrace::ForDuration,
															 CapsuleHitResult,
															 true);

															if (!CapsuleHitResult.bBlockingHit &&
																abs((FloorResult.HitResult.ImpactPoint - WallEndPoint).
																	Z) >= 50.f)
															{
																FinalResult.bLandable     = false;
																FinalResult.bVaultable    = false;
																FinalResult.FirstPosition = FirstPosition;
																FinalResult.DepthPosition = WallEndPoint;
																FinalResult.FloorPosition = FloorResult.HitResult.
																		ImpactPoint;
																FinalResult.WallNormal        = LastForwardNormal;
																FinalResult.Height            = CachedHeight;
																FinalResult.Depth             = CachedVaultDepth;
																FinalResult.BottomEmptyHeight = 0;

																WallHitResult = EParkourWallHitResult::Vault;
																return true;
															}
														}
													}
												}
												else
												{
													break;
												}
											}
										}
									}
								}
							}

							// 마지막 루프까지 다 돌았는데 걸을 수 없으면 벽을 넘는건 불가능
							else if (HitCounter == WallTrace_Iteration)
							{
								// Wall 구조체 채우기 (필요한 정보는 첫번째 Hit위치, 벽 법선)
								FinalResult.bLandable         = false;
								FinalResult.bVaultable        = false;
								FinalResult.FirstPosition     = FirstHitResult.ImpactPoint;
								FinalResult.DepthPosition     = FVector::ZeroVector;
								FinalResult.FloorPosition     = FVector::ZeroVector;
								FinalResult.WallNormal        = FirstHitResult.Normal;
								FinalResult.Height            = 0;
								FinalResult.Depth             = 0;
								FinalResult.BottomEmptyHeight = 0;

								WallHitResult = EParkourWallHitResult::WallRun;

								return FirstHitResult.bBlockingHit;
							}
						}
					}
				}
				// 첫 Hit 결과 저장
				else
				{
					bFirstHitAssigned = true;
					FirstHitResult    = BoxTraceResult;
				}
			}
		}
	}

	// return
	WallHitResult = EParkourWallHitResult::None;
	return false;
}

bool UParkourComponent::TryTakeParkourAction()
{
	if (CanTraceWall(LastAccelerationDirection))
	{
		EParkourWallHitResult WallHitResult;
		if (TraceWall(
					  GetCharacter()->GetActorLocation(),
					  CurrentInputDirection,
					  WallTrace_Height,
					  WallTrace_Iteration,
					  WallHitResult,
					  CachedWallHitResult))
		{
			if (WallHitResult == EParkourWallHitResult::WallRun)
			{
				// UE_LOG(LogTemp, Display, TEXT("WallRun"))
				// return StartWallRun();
			}
			else if (WallHitResult == EParkourWallHitResult::Vault && bJumpActionTriggered)
			{
				// UE_LOG(LogTemp, Display, TEXT("Vault"))
				if (StartMotionWarping(CachedWallHitResult, EParkourWallHitResult::Vault))
				{
					SetParkourState(EParkourState::Vault);
					return true;
				}
			}
			else if (WallHitResult == EParkourWallHitResult::Step && bJumpActionTriggered)
			{
				// UE_LOG(LogTemp, Display, TEXT("Step On"))
				if (StartMotionWarping(CachedWallHitResult, EParkourWallHitResult::Step))
				{
					SetParkourState(EParkourState::StepUp);
					return true;
				}
			}
			else if (WallHitResult == EParkourWallHitResult::Mantle && bJumpActionTriggered)
			{
				if (StartMotionWarping(CachedWallHitResult, EParkourWallHitResult::Mantle))
				{
					SetParkourState(EParkourState::Mantle);

					LocomotionComponent->SetMovementType(EMovementType::LedgeOrHang);
					GetMovementComponent()->SetMovementMode(MOVE_Flying);

					return true;
				}
			}
		}
	}
	return false;
}

void UParkourComponent::TryMoveOnLedge(const FVector2D& Direction)
{
	// GetCharacter()->AddMovementInput(GetCharacter()->GetActorRotation().Quaternion().GetUpVector(), Direction.X);
	// FVector::VectorPlaneProject(UKismetMathLibrary::GetRightVector(
	// 	                            FRotator(0, GetCharacter()->GetControlRotation().Yaw, 0)),
	//                             GetCharacter()->GetActorForwardVector());;
	//

	//
	// 	Loc,
	// 	Rot
	// );
	if (CanMoveOnLedge(Direction))
	{
		GetCharacter()->AddMovementInput(
										 UKismetMathLibrary::GetRightVector(FRotator(0,
																				GetCharacter()->GetActorRotation().
																				Yaw,
																				0))
										 // CurrentInputDirection)
										 ,
										 Direction.Y
										);
	}
}

bool UParkourComponent::CanMoveOnLedge(const FVector2D& Direction)
{
	const FVector HandLocation = GetSkeletalMeshComponent()->GetSocketLocation(Direction.Y > 0 ? "hand_r" : "hand_l");
	const FVector TraceStart   = FVector(
										 HandLocation.X,
										 HandLocation.Y,
										 CachedWallHitResult.FirstPosition.Z)
			+ GetCharacter()->GetActorRightVector() * 30.f * Direction.Y + GetCharacter()->
			GetActorForwardVector() * 20.f;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult HitResult;

	UKismetSystemLibrary::SphereTraceSingle(
											GetWorld(),
											TraceStart + FVector::UpVector * 50.f,
											TraceStart + FVector::UpVector * -50.f,
											20.f,
											TraceTypeQuery1,
											false,
											ActorsToIgnore,
											EDrawDebugTrace::ForOneFrame,
											HitResult,
											true,
											FLinearColor::White,
											FLinearColor::Green);

	if (HitResult.bBlockingHit)
	{
		CachedWallHitResult.FirstPosition = HitResult.ImpactPoint;
		CachedWallHitResult.DepthPosition = HitResult.ImpactPoint + HitResult.Normal * 35.f;
		CachedWallHitResult.FloorPosition = HitResult.ImpactPoint + HitResult.Normal * 35.f;
		CachedWallHitResult.WallNormal    = HitResult.Normal;
	}

	return HitResult.bBlockingHit;
}

void UParkourComponent::MoveOnLedge(float MoveAmount, FVector& Location, FRotator& Rotation)
{
	if ((CurrentParkourState == EParkourState::Ledge || CurrentParkourState == EParkourState::Hang) &&
		LocomotionComponent->CurrentMovementType == EMovementType::LedgeOrHang)
	{
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetOwner());
		FHitResult FirstLineTraceResult;
		FHitResult SecondLineTraceResult;
		FHitResult ThirdHitResult;

		FVector UpAndDirectionStart = GetCharacter()->GetActorLocation() + GetCharacter()->GetActorRightVector() *
				MoveAmount + FVector::UpVector * 60.f;


		UKismetSystemLibrary::LineTraceSingle(
											  GetWorld(),
											  UpAndDirectionStart,
											  UpAndDirectionStart + GetCharacter()->GetActorForwardVector() * 250.f,
											  TraceTypeQuery1,
											  false,
											  ActorsToIgnore,
											  EDrawDebugTrace::ForOneFrame,
											  FirstLineTraceResult,
											  true);

		if (FirstLineTraceResult.bBlockingHit)
		{
			UKismetSystemLibrary::LineTraceSingle(
												  GetWorld(),
												  GetCharacter()->GetActorLocation() + FVector::UpVector * 60.f,
												  GetCharacter()->GetActorLocation() + FVector::UpVector * 60.f +
												  GetCharacter()->GetActorForwardVector()
												  * 250.f,
												  TraceTypeQuery1,
												  false,
												  ActorsToIgnore,
												  EDrawDebugTrace::ForOneFrame,
												  SecondLineTraceResult,
												  true);

			FVector HeadLocation = GetSkeletalMeshComponent()->GetSocketLocation("head");

			UKismetSystemLibrary::SphereTraceSingle(
													GetWorld(),
													GetCharacter()->GetActorRightVector() * MoveAmount + FVector(
													 HeadLocation.X,
													 HeadLocation.Y,
													 (GetSkeletalMeshComponent()->GetComponentLocation() +
														 FVector::UpVector * GetCapsuleComponent()->
														 GetScaledCapsuleHalfHeight() + 100).Z),
													GetCharacter()->GetActorRightVector() * MoveAmount + FVector(
													 HeadLocation.X,
													 HeadLocation.Y,
													 (GetSkeletalMeshComponent()->GetComponentLocation() +
														 FVector::UpVector * GetCapsuleComponent()->
														 GetScaledCapsuleHalfHeight() + 100).Z),
													20.f,
													TraceTypeQuery1,
													false,
													ActorsToIgnore,
													EDrawDebugTrace::ForOneFrame,
													ThirdHitResult,
													true);

			if (!ThirdHitResult.bBlockingHit)
			{
				LedgeMovementDirection = MoveAmount < 0 ? EMovementDirection::Left : EMovementDirection::Right;
			}


			FVector  LedgePos;
			FRotator LedgeRot;
			FindLedgeData(MoveAmount,
						  FirstLineTraceResult.ImpactPoint,
						  FirstLineTraceResult.ImpactNormal,
						  SecondLineTraceResult.ImpactNormal,
						  LedgePos,
						  LedgeRot);

			Location = LedgePos;
			Rotation = LedgeRot;
		}
	}
}

void UParkourComponent::FindLedgeData(float          MoveAmount, const FVector& LedgePosition, const FVector& LedgeNormal,
									  const FVector& HitNormal, FVector&        NewLocation, FRotator& NewRotator) const
{
	const FVector LocalLedge = LedgePosition + LedgeNormal * 50.f - GetCharacter()->GetActorLocation().GetSafeNormal() *
			MoveAmount;

	NewLocation = GetCharacter()->GetActorLocation() + LocalLedge = FVector(LocalLedge.X, LocalLedge.Y, 0);

	const FRotator LocalRot = UKismetMathLibrary::MakeRotFromX(-LedgeNormal);
	NewRotator              = FRotator(LocalRot.Pitch,
									   (UKismetMathLibrary::MakeRotFromX(-HitNormal).Yaw + LocalRot.Yaw) / 2,
									   LocalRot.Roll);
}

bool UParkourComponent::FindAnimSets(const FWallHitResult& WallHitResult, EParkourWallHitResult ActionType,
									 FParkourAnim&         ParkourAnim)
{
	float CapsuleRadius, CapsuleHalfHeight;
	GetCapsuleSize(true, CapsuleRadius, CapsuleHalfHeight);

	// 미리 지정된 데이터에셋들에서 알맞은 데이터를 찾아야 함
	for (UParkourAction* DataAsset : ParkourDataAssets)
	{
		// TODO: 점프하면서 파쿠르를 실행하려면 여기서 제어

		// 사용하려는 모션 타입과 일치하고 활성화가 되어있으면 이 에셋을 쓰기로 결정
		if (DataAsset && DataAsset->bEnabled && ActionType == DataAsset->WallHitType)
		{
			// 아래 상태에서는 ActionType을 만족하지 않음 (예외 처리)
			switch (DataAsset->ActionType)
			{
			case EParkourType::Default:
				if (!GetMovementComponent()->IsMovingOnGround())
					continue;
				break;
			case EParkourType::Reach:
				// if (CurrentParkourState != EParkourState::WallRun)
				// 	continue;

				// if ((WallHitResult.FirstPosition.Z - (GetCharacter()->GetActorLocation().Z + CapsuleHalfHeight)) >
				// 	DataAsset->ReachDistance)
				// 	continue;
				break;
			case EParkourType::Catch:
				if (CurrentParkourState == EParkourState::WallRun)
					continue;
				if (GetMovementComponent()->IsMovingOnGround())
					continue;
				if (!IsValueInRange(DataAsset->CatchHeightDistance,
									trunc((WallHitResult.FirstPosition.Z - (GetCharacter()->GetActorLocation().Z +
											  CapsuleHalfHeight)))))
					continue;
				break;
			}

			// 아래에서는 데이터에셋에 지정된 몽타주를 재생하기에 올바른 범위를 가지는지 확인한다.
			const bool HeightCheck   = IsValueInRange(DataAsset->RangeSetup.HeightRange, WallHitResult.Height);
			const bool DistanceCheck = IsValueInRange(DataAsset->RangeSetup.DistanceRange,
													  trunc((GetCharacter()->GetActorLocation() + WallHitResult.
																WallNormal * CapsuleRadius - WallHitResult.FirstPosition).
															Size2D()));
			const bool DepthCheck = ActionType == EParkourWallHitResult::Vault
										? IsValueInRange(DataAsset->RangeSetup.DepthRange, WallHitResult.Depth)
										: true;
			const bool VelocityCheck = DataAsset->ActionType == EParkourType::Default
										   ? IsValueInRange(DataAsset->RangeSetup.VelocityRange,
															trunc(GetMovementComponent()->GetLastUpdateVelocity().
																  Size2D()))
										   : true;

			// 위 조건들을 모두 만족하면 몽타주들을 셔플해준다. (랜덤하게 재생해주기 위함)
			if (HeightCheck && DistanceCheck && DepthCheck && VelocityCheck)
			{
				const int32 NumShuffles = DataAsset->ParkourAnims.Num() - 1;

				for (int32 i = 0; i < NumShuffles; ++i)
				{
					const int32 SwapIdx = FMath::RandRange(i, NumShuffles);
					DataAsset->ParkourAnims.Swap(i, SwapIdx);
				}

				for (const FParkourAnim& AnimData : DataAsset->ParkourAnims)
				{
					// 활성화 되어있고 모션워핑 변수가 채워지고 몽타주가 존재하면 가장 앞의 에셋을 사용하기로 결정
					if (AnimData.bEnabled && !AnimData.MotionWarpingSetups.IsEmpty() && AnimData.MontageToPlay)
					{
						ParkourAnim = AnimData;

						UE_LOG(LogTemp, Warning, TEXT("Data Asset : %s"), *DataAsset->GetName());
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool UParkourComponent::CheckLedgeHasSurface()
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	FHitResult SideHitResult;

	// LineTrace 왼쪽
	FVector LineTraceStart = GetSkeletalMeshComponent()->GetSocketLocation("ball_l");

	UKismetSystemLibrary::LineTraceSingle(
										  GetWorld(),
										  LineTraceStart,
										  LineTraceStart + GetCharacter()->GetActorForwardVector() * 40,
										  TraceTypeQuery1,
										  false,
										  ActorsToIgnore,
										  EDrawDebugTrace::ForOneFrame,
										  SideHitResult,
										  true
										 );

	bHasLedgeSurface_L = SideHitResult.bBlockingHit;

	// LineTrace 오른쪽
	SideHitResult.Init();
	LineTraceStart = GetSkeletalMeshComponent()->GetSocketLocation("ball_r");

	UKismetSystemLibrary::LineTraceSingle(
										  GetWorld(),
										  LineTraceStart,
										  LineTraceStart + GetCharacter()->GetActorForwardVector() * 40,
										  TraceTypeQuery1,
										  false,
										  ActorsToIgnore,
										  EDrawDebugTrace::ForOneFrame,
										  SideHitResult,
										  true
										 );

	bHasLedgeSurface_R = SideHitResult.bBlockingHit;

	return bHasLedgeSurface_L || bHasLedgeSurface_R;
}

bool UParkourComponent::StartMotionWarping(const FWallHitResult& WallHitResult, EParkourWallHitResult ActionType)
{
	FParkourAnim ParkourAnimSet;

	// 재생할 모션이 있는지 확인
	if (FindAnimSets(CachedWallHitResult, ActionType, ParkourAnimSet))
	{
		TArray<FVector> Local_Last;
		// int Index = 0;

		// 불러온 데이터에셋에서 모션워핑 관련 데이터를 채워준다.
		for (const FMotionWarpingSetup AnimSet : ParkourAnimSet.MotionWarpingSetups)
		{
			const FName    WarpTargetName    = AnimSet.MotionWarpingTarget;
			const FRotator WarpTargetRotator = UKismetMathLibrary::MakeRotFromX(-WallHitResult.WallNormal);
			FVector        Local_Warp_Target = ApplyOffset(UKismetMathLibrary::MakeRotFromX(-WallHitResult.WallNormal),
														   AnimSet.LocalSpaceOffset);

			// 모션워핑 종류에 따라 목표 지점 설정
			switch (AnimSet.Space)
			{
			case EParkourMotionWarpingTarget::WallTopStart:
				Local_Warp_Target += WallHitResult.FirstPosition;
				break;
			case EParkourMotionWarpingTarget::WallTopEnd:
				Local_Warp_Target += WallHitResult.DepthPosition;
				break;
			case EParkourMotionWarpingTarget::WallLand:
				Local_Warp_Target += WallHitResult.FloorPosition;
				break;
			case EParkourMotionWarpingTarget::WallStartHeight:
				Local_Warp_Target += FVector(WallHitResult.FirstPosition.X,
											 WallHitResult.FirstPosition.Y,
											 GetCharacter()->GetActorLocation().Z);
				break;
			}

			/*// if (Index == ParkourAnimSet.MotionWarpingSetups.Num() - 1)
			// {
			// 	TArray<FHitResult> AllHits;
			// 	TArray<FHitResult> Hits;
			// 	if (LocomotionComponent->CircularDetection(Local_Warp_Target,
			// 	                                           FVector::ZeroVector,
			// 	                                           20,
			// 	                                           50.f,
			// 	                                           5.f,
			// 	                                           60.f,
			// 	                                           360.f,
			// 	                                           0.f,
			// 	                                           FLinearColor::White,
			// 	                                           Hits,
			// 	                                           AllHits))
			// 	{
			// 		for (const FHitResult& Hit : Hits)
			// 		{
			// 			Local_Last.Add(Hit.ImpactPoint);
			// 		}
			//
			// 		Local_Warp_Target = UKismetMathLibrary::GetVectorArrayAverage(Local_Last);
			//
			// 		DrawDebugLine(GetWorld(), GetCharacter()->GetActorLocation(), Local_Warp_Target, FColor::Orange,
			// 		              false, 10.f, 0, 3.f);
			// 	}
			// }*/

			// 모션워핑 타깃 설정
			MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(
																	   WarpTargetName,
																	   FTransform(WarpTargetRotator, Local_Warp_Target));

			// Index++;
		}

		GetCharacter()->GetController()->SetIgnoreMoveInput(true);
		GetMovementComponent()->SetMovementMode(MOVE_Flying);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		LocomotionComponent->CurrentCharacterState = ECharacterState::Stable;

		CachedMontage = ParkourAnimSet.MontageToPlay;

		GetAnimInstance()->Montage_Play(CachedMontage);

		LocomotionComponent->SetCharacterRotation(
												  FRotator(0,
														   UKismetMathLibrary::MakeRotFromX(-WallHitResult.WallNormal).Yaw,
														   0),
												  0.f,
												  0.f);
		SetRotationValues(FRotator(0, UKismetMathLibrary::MakeRotFromX(-WallHitResult.WallNormal).Yaw, 0));

		return true;
	}
	return false;
}

bool UParkourComponent::StartMotionWarpingWithDirection(const FWallHitResult&    WallHitResult,
														const EMovementDirection Direction)
{
	if (ParkourDataAsset_Direction.Contains(Direction))
	{
		FParkourAnim ParkourAnim = (*ParkourDataAsset_Direction.Find(Direction))->ParkourAnims[0];
		for (const FMotionWarpingSetup AnimSet : ParkourAnim.MotionWarpingSetups)
		{
			const FName    WarpTargetName    = AnimSet.MotionWarpingTarget;
			const FRotator WarpTargetRotator = UKismetMathLibrary::MakeRotFromX(-WallHitResult.WallNormal);
			FVector        Local_Warp_Target = ApplyOffset(UKismetMathLibrary::MakeRotFromX(-WallHitResult.WallNormal),
														   AnimSet.LocalSpaceOffset);

			// 모션워핑 종류에 따라 목표 지점 설정
			switch (AnimSet.Space)
			{
			case EParkourMotionWarpingTarget::WallTopStart:
				Local_Warp_Target += WallHitResult.FirstPosition;
				break;
			case EParkourMotionWarpingTarget::WallTopEnd:
				Local_Warp_Target += WallHitResult.DepthPosition;
				break;
			case EParkourMotionWarpingTarget::WallLand:
				Local_Warp_Target += WallHitResult.FloorPosition;
				break;
			case EParkourMotionWarpingTarget::WallStartHeight:
				Local_Warp_Target += FVector(WallHitResult.FirstPosition.X,
											 WallHitResult.FirstPosition.Y,
											 GetCharacter()->GetActorLocation().Z);
				break;
			}

			// 모션워핑 타깃 설정
			MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(
																	   WarpTargetName,
																	   FTransform(WarpTargetRotator, Local_Warp_Target));

			// Index++;
		}
		GetCharacter()->GetController()->SetIgnoreMoveInput(true);
		GetMovementComponent()->SetMovementMode(MOVE_Flying);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		LocomotionComponent->CurrentCharacterState = ECharacterState::Stable;

		CachedMontage = ParkourAnim.MontageToPlay;

		GetAnimInstance()->Montage_Play(CachedMontage);

		LocomotionComponent->SetCharacterRotation(
												  FRotator(0,
														   UKismetMathLibrary::MakeRotFromX(-WallHitResult.WallNormal).Yaw,
														   0),
												  0.f,
												  0.f);
		SetRotationValues(FRotator(0, UKismetMathLibrary::MakeRotFromX(-WallHitResult.WallNormal).Yaw, 0));

		return true;
	}
	return false;
}

void UParkourComponent::SetParkourState(const EParkourState TargetParkourState)
{
	CurrentParkourState = TargetParkourState;
}

void UParkourComponent::SetRotationValues(const FRotator& Rotator) const
{
	LocomotionComponent->CharacterSmoothRotation     = Rotator;
	LocomotionComponent->CharacterLastTargetRotation = Rotator;
	LocomotionComponent->LastVelocityRotation        = Rotator;
	LocomotionComponent->LastAirRotation             = Rotator;
	LocomotionComponent->LastInputRotation           = Rotator;
}

bool UParkourComponent::TryHop()
{
	if (CurrentParkourState
		== EParkourState::Ledge || CurrentParkourState == EParkourState::Hang)
	{
		FHitResult      HitResult;
		FHitResult      CurrentLedgeHitResult;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetOwner());

		UKismetSystemLibrary::LineTraceSingle(
											  GetWorld(),
											  CachedWallHitResult.FirstPosition + FVector::UpVector + 50.f,
											  CachedWallHitResult.FirstPosition + FVector::UpVector - 50.f,
											  TraceTypeQuery1,
											  false,
											  ActorsToIgnore,
											  EDrawDebugTrace::ForOneFrame,
											  CurrentLedgeHitResult,
											  true);

		// 옆으로 점프를 하려고 시도하면 옆에 뛸 수 있는 난간이 있는지 체크
		if (CurrentInputDirection.Size() > 0.f)
		{
			const FVector RootLocation = GetSkeletalMeshComponent()->GetSocketLocation("head");

			const FVector TraceStart = RootLocation
					+ GetCharacter()->GetActorRightVector() * LocomotionComponent->MovementInput.Y *
					(200.f)
					+ GetCharacter()->GetActorUpVector() * LocomotionComponent->MovementInput.X * (
						200.f)
					+ GetCharacter()->GetActorForwardVector() * 40.f;

			// 총 10번 boxtrace를 진행
			HitResult.Init();

			// 이동 방향으로 BoxTrace 진행
			UKismetSystemLibrary::SphereTraceSingle(
													GetWorld(),
													TraceStart,
													TraceStart,
													40.f,
													TraceTypeQuery1,
													false,
													ActorsToIgnore,
													EDrawDebugTrace::ForDuration,
													HitResult,
													true);

			// 난간인지 확인
			if (HitResult.bBlockingHit)
			{
				FHitResult LineTraceResult;

				UKismetSystemLibrary::LineTraceSingle(
													  GetWorld(),
													  HitResult.ImpactPoint - HitResult.Normal * 20.f + FVector::UpVector *
													  50.f,
													  HitResult.ImpactPoint - HitResult.Normal * 20.f - FVector::UpVector *
													  50.f,
													  TraceTypeQuery1,
													  false,
													  ActorsToIgnore,
													  EDrawDebugTrace::ForDuration,
													  LineTraceResult,
													  true,
													  FColor::Cyan,
													  FColor::Purple);

				if (LineTraceResult.IsValidBlockingHit())
				{
					DrawDebugBox(
								 GetWorld(),
								 LineTraceResult.ImpactPoint,
								 FVector::OneVector * 5.f,
								 FColor::Emerald,
								 false,
								 10.f,
								 0,
								 5.f
								);
					FWallHitResult ReachWallHitResult;
					ReachWallHitResult.bLandable     = false;
					ReachWallHitResult.bVaultable    = false;
					ReachWallHitResult.FirstPosition = LineTraceResult.ImpactPoint;
					ReachWallHitResult.DepthPosition = LineTraceResult.ImpactPoint + LineTraceResult.Normal * 35.f;
					ReachWallHitResult.FloorPosition = LineTraceResult.ImpactPoint + LineTraceResult.Normal * 35.f;
					ReachWallHitResult.WallNormal    = HitResult.Normal;
					ReachWallHitResult.Height        = LineTraceResult.ImpactPoint.Z - GetCharacter()->GetActorLocation().
							Z;
					ReachWallHitResult.Depth             = 0;
					ReachWallHitResult.BottomEmptyHeight = 0;

					if (StartMotionWarpingWithDirection(ReachWallHitResult,
														LocomotionComponent->MovementInput.Y > 0
															? EMovementDirection::Right
															: EMovementDirection::Left))
					{
						return true;
					}
				}
			}
		}


		UKismetSystemLibrary::CapsuleTraceSingle(
												 GetWorld(),
												 GetCharacter()->GetActorLocation() + GetCharacter()->
												 GetActorForwardVector() * 85.f + FVector(
												  0,
												  0,
												  GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 100),
												 GetCharacter()->GetActorLocation() + GetCharacter()->
												 GetActorForwardVector() * 85.f + FVector(
												  0,
												  0,
												  GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 100),
												 25.f,
												 80.f,
												 TraceTypeQuery1,
												 false,
												 ActorsToIgnore,
												 EDrawDebugTrace::ForDuration,
												 HitResult,
												 true
												);

		if (!HitResult.bBlockingHit)
		{
			if (TryTakeParkourAction())
			{
				return true;
			}

			// GetCharacter()->Jump();
			SetParkourState(EParkourState::None);
			WallRunState = EWallRunState::None;

			// ResetMotionWarping();

			LocomotionComponent->SetMovementType(EMovementType::Ground);

			// GetCharacter()->GetController()->SetIgnoreMoveInput(false);

			FFindFloorResult FloorResult;
			GetMovementComponent()->ComputeFloorDist(GetCapsuleComponent()->GetComponentLocation(),
													 1000.f,
													 1000.f,
													 FloorResult,
													 5.f);

			GetMovementComponent()->SetMovementMode(MOVE_Falling);
			LocomotionComponent->CurrentCharacterState = FloorResult.bBlockingHit && FloorResult.FloorDist < 80.f
															 ? ECharacterState::Stable
															 : ECharacterState::Falling;
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

			GetCharacter()->GetController()->SetIgnoreMoveInput(false);
			return false;
		}
	}

	return false;
}

bool UParkourComponent::IsValueInRange(const FInt32Range& Range, const int Value)
{
	return Range.Contains(Value);
}

FVector UParkourComponent::ApplyOffset(const FRotator& Rotator, const FVector& Vector)
{
	;
	return
			Rotator.Quaternion().GetForwardVector() * Vector.X +
			Rotator.Quaternion().GetRightVector() * Vector.Y +
			Rotator.Quaternion().GetUpVector() * Vector.Z;
}

void UParkourComponent::ResetMotionWarping() const
{
	GetCharacter()->GetController()->SetIgnoreMoveInput(false);
}
