// CopyRight KGCA - Team RedCoke


#include "MoKong/Public/Character/MKPlayer.h"

#include "ATPCCameraComponent.h"
#include "ATPCCameraLockOnTargetObject.h"
#include "Ability/MKAbilitySystemComponent.h"
#include "Character/MokongEnemy.h"
#include "CombatSystem/Components/CombatSystemComp.h"
#include "Component/FootStepSFXComponent.h"
#include "Component/LocomotionComponent.h"
#include "Data/TransformData.h"
#include "Engine/OverlapResult.h"

// Sets default values
AMKPlayer::AMKPlayer()
{
	PlayerCameraComponent = CreateDefaultSubobject<UATPCCameraComponent>(TEXT("플레이어캠"));
	PlayerCameraComponent->SetupAttachment(GetRootComponent());

	HonBaekMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("혼백"));
	HonBaekMeshComponent->SetupAttachment(GetMesh());
	HonBaekMeshComponent->SetVisibility(false);

	GourdMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gourd"));
	GourdMeshComponent->SetupAttachment(GetMesh(), "gourd_main");

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

void AMKPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	CombatComponent->InitializeCombatSystem(GetMesh());
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
		break;
	case EPlayerTransformTypes::HonBaek:
		TransformToHonBaek();
		break;
	case EPlayerTransformTypes::Animal:
		UE_LOG(LogTemp, Warning, TEXT( "아직 구현되지 않음." ));
		break;
	case EPlayerTransformTypes::Transform:
		TransformToByeonSin();
		break;
	}


}

void AMKPlayer::TransformToSelf()
{
	GetMesh()->SetSkeletalMesh(WukongMesh.Get());
	GetMesh()->SetAnimInstanceClass(WukongAnimInstance.Get());
	CombatComponent->SwapWeapon(EWeaponType::BoStaff, GetMesh());

	GetMesh()->SetVisibility(true, true);
	GetMesh()->SetVisibility(false, false);
}

void AMKPlayer::TransformToHonBaek() {}

void AMKPlayer::TransformToByeonSin()
{
	if (!CachedTransformData)
	{
		UE_LOG(LogTemp, Warning, TEXT( "변신 데이터가 없습니다." ));
		return;
	}

	GetMesh()->SetSkeletalMesh(CachedTransformData->AvatarMesh.Get());
	GetMesh()->SetAnimInstanceClass(CachedTransformData->AnimationInstance.Get());
	CombatComponent->SwapWeapon(CachedTransformData->WeaponType, GetMesh());
	GetMesh()->SetVisibility(false, true);

	GetMesh()->SetVisibility(true, false);
}

void AMKPlayer::SetMeshParts(const EMeshParts MeshPart, USkeletalMesh* NewMesh)
{
	switch (MeshPart)
	{
	case EMeshParts::Helmet:
		Helmet->SetSkeletalMesh(NewMesh);
		break;
	case EMeshParts::Suit:
		Suit->SetSkeletalMesh(NewMesh);
		break;
	case EMeshParts::Shoes:
		Shoes->SetSkeletalMesh(NewMesh);
		break;
	case EMeshParts::Gloves:
		Gloves->SetSkeletalMesh(NewMesh);
		break;
	case EMeshParts::Head:
		Head->SetSkeletalMesh(NewMesh);
		break;
	}
}
