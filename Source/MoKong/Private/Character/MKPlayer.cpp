// CopyRight KGCA - Team RedCoke


#include "MoKong/Public/Character/MKPlayer.h"

#include "ATPCCameraComponent.h"
#include "ATPCCameraLockOnTargetObject.h"
#include "Ability/MKAbilitySystemComponent.h"
#include "Character/MokongEnemy.h"
#include "CombatSystem/Components/CombatSystemComp.h"
#include "Component/FootStepSFXComponent.h"
#include "Component/LocomotionComponent.h"
#include "Engine/OverlapResult.h"

// Sets default values
AMKPlayer::AMKPlayer()
{
	PlayerCameraComponent = CreateDefaultSubobject<UATPCCameraComponent>(TEXT("플레이어캠"));
	PlayerCameraComponent->SetupAttachment(GetRootComponent());

	TransformMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("변신메시"));
	TransformMeshComponent->SetupAttachment(GetRootComponent());
	TransformMeshComponent->SetVisibility(false);
	TransformMeshComponent->SetActive(false);

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

		// 플레이어가 보는 방향으로 가장 가까운 적을 찾는다.
		float NearestFacingDot = -1.f;
		for (const FOverlapResult& HitResult : HitResults)
		{
			if (AActor* Actor = HitResult.GetActor())
			{

				const FVector ToActor = Actor->GetActorLocation() - Origin;
				const float   Dot     = FVector::DotProduct(ToActor.GetSafeNormal(), Forward);

				if (Dot > NearestFacingDot)
				{
					NearestFacingDot = Dot;
					ClosestActor     = Actor;
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

void AMKPlayer::Transform()
{
	// Try to transform
	const bool bShouldRevert = AbilitySystemComponent->
			HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Status.Transform"));

	ChangeActiveMesh(bShouldRevert ? EPlayerTransformTypes::Self : EPlayerTransformTypes::Transform);
}

void AMKPlayer::ChangeActiveMesh(const EPlayerTransformTypes TransformType)
{
	switch (TransformType)
	{
	case EPlayerTransformTypes::Self:
		TransformMeshComponent->SetVisibility(false);
		TransformMeshComponent->SetActive(false);
		GetMesh()->SetActive(true);
		GetMesh()->SetVisibility(true, true);
		CombatComponent->SwapWeapon(EWeaponType::BoStaff, GetMesh());
		break;
	case EPlayerTransformTypes::HonBaek:
		break;
	case EPlayerTransformTypes::Animal:
		break;
	case EPlayerTransformTypes::Transform:
		TransformMeshComponent->SetVisibility(true);
		TransformMeshComponent->SetActive(true);
		GetMesh()->SetActive(false);
		GetMesh()->SetVisibility(false, true);
		CombatComponent->SwapWeapon(EWeaponType::Sword, TransformMeshComponent.Get());
		break;
	}


}
