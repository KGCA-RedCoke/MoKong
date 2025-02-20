// CopyRight KGCA - Team RedCoke


#include "MoKong/Public/Character/MKPlayer.h"

#include "ATPCCameraComponent.h"
#include "ATPCCameraLockOnTargetObject.h"
#include "Character/MokongEnemy.h"
#include "Component/FootStepSFXComponent.h"
#include "Component/LocomotionComponent.h"
#include "Engine/OverlapResult.h"

// Sets default values
AMKPlayer::AMKPlayer()
{
	PlayerCameraComponent = CreateDefaultSubobject<UATPCCameraComponent>(TEXT("플레이어캠"));
	PlayerCameraComponent->SetupAttachment(GetRootComponent());

	GourdMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("호리병메시"));
	GourdMeshComponent->SetupAttachment(GetMesh(), FName("gourd_main"));

	FootstepComponent = CreateDefaultSubobject<UFootStepSFXComponent>(TEXT("FootstepComponent"));

	OnPlayerStateChanged.AddUniqueDynamic(this, &AMKPlayer::PlayerStateChaneDelegate);
}

void AMKPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerCameraComponent)
	{
		if (UATPCCameraLockOnTargetObject* LockOnComp = PlayerCameraComponent->GetCameraLockOnTargetObject())
		{
			LockOnComp->OnTargetChangeDelegate.AddUniqueDynamic(this, &AMKPlayer::OnLockOnTargetChange);
		}

	}
}

void AMKPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

AMokongEnemy* AMKPlayer::GetNearestEnemy(float Distance)
{
	AActor*                ClosestActor = nullptr;
	TArray<FOverlapResult> HitResults;

	const FVector Origin  = GetActorLocation();
	const FVector Forward = GetControlRotation().Vector();

	const bool bAnythingHit = GetWorld()->OverlapMultiByObjectType(
																   HitResults,
																   Origin + (Distance * 0.5f) * Forward,
																   FQuat::Identity,
																   ECollisionChannel::ECC_GameTraceChannel2,
																   FCollisionShape::MakeSphere(Distance));

	if (bAnythingHit)
	{
		float DistanceFromNearestActor = TNumericLimits<float>::Max();

		for (const FOverlapResult& HitResult : HitResults)
		{
			if (AActor* Actor = HitResult.GetActor())
			{
				const float DistanceFromActorToCheck = (Origin - Actor->GetActorLocation()).SizeSquared();
				if (DistanceFromActorToCheck < DistanceFromNearestActor)
				{
					ClosestActor             = Actor;
					DistanceFromNearestActor = DistanceFromActorToCheck;
				}
			}
		}
	}

	return Cast<AMokongEnemy>(ClosestActor);
}

void AMKPlayer::PlayerStateChaneDelegate(EPlayerState NewState)
{
	if (NewState == CurrentPlayerState)
		return;

	CurrentPlayerState = NewState;

	switch (CurrentPlayerState)
	{
	case EPlayerState::Stable:
		break;
	case EPlayerState::Battle:
		break;
	case EPlayerState::Action:
		break;
	case EPlayerState::Dead:
		break;
	}
}

void AMKPlayer::OnLockOnTargetChange(AActor* NewTarget, EATPCChangeTargetReason ChangeTargetReason)
{
	switch (ChangeTargetReason)
	{
	case EATPCChangeTargetReason::SetNew:
		if (NewTarget)
		{
			TargetEnemy = Cast<AMokongEnemy>(NewTarget);
			TargetEnemy->ShowLockOnWidget(true);
		}
		else
		{
			TargetEnemy->ShowLockOnWidget(false);
			TargetEnemy = nullptr;
		}
		break;
	case EATPCChangeTargetReason::LostByExitCameraMode:
	case EATPCChangeTargetReason::LostByDistance:
	case EATPCChangeTargetReason::LostByVisible:
	case EATPCChangeTargetReason::LostByPlayerInput:
		TargetEnemy->ShowLockOnWidget(false);
		TargetEnemy = nullptr;
		break;
	}
	LocomotionComponent->SetPose(TargetEnemy ? EPose::Aiming : EPose::Neutral);
}

void AMKPlayer::PreAttack_Implementation(float EffectLevel)
{
	Super::PreAttack_Implementation(EffectLevel);
}

AActor* AMKPlayer::GetTargetActor_Implementation() const
{
	return TargetEnemy.Get();
}

bool AMKPlayer::IsLockingOn() const
{
	return IsValid(PlayerCameraComponent->GetCameraLockOnTargetObject()->GetTargetActor());
}
