// CopyRight KGCA - Team RedCoke


#include "MoKong/Public/Character/MKPlayer.h"

#include "ATPCCameraComponent.h"
#include "ATPCCameraLockOnTargetObject.h"
#include "Ability/MKAbilitySystemComponent.h"
#include "Character/MokongEnemy.h"
#include "CombatSystem/Components/CombatSystemComp.h"
#include "Component/FootStepSFXComponent.h"
#include "Component/InventorySystemComponent.h"
#include "Component/LocomotionComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/OverlapResult.h"
#include "InventorySystem/Public/Actor/Gourd.h"
#include "Player/MKPlayerController.h"

// Sets default values
AMKPlayer::AMKPlayer()
{
	PlayerCameraComponent = CreateDefaultSubobject<UATPCCameraComponent>(TEXT("플레이어캠"));
	PlayerCameraComponent->SetupAttachment(GetRootComponent());

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

		if (!GourdClass.IsNull())
		{
			GourdActor = GetWorld()->SpawnActor<AGourd>(GourdClass.LoadSynchronous());

			if (GourdActor)
			{
				GourdActor->AttachToComponent(GetMesh(),
											  FAttachmentTransformRules::SnapToTargetIncludingScale,
											  "gourd_main");
			}
		}
	}
}

void AMKPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CombatComponent->InitializeCombatSystem(GetMesh());

	if (auto* ControllerRef = Cast<AMKPlayerController>(NewController))
	{
		InventorySystemComponent = ControllerRef->Inventory_Items;
		ensure(InventorySystemComponent);

		OnAbilityCharacterDie.AddUniqueDynamic(ControllerRef, &AMKPlayerController::ResetState);
	}
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

void AMKPlayer::PreAttack_Implementation(TSubclassOf<UGameplayEffect> Effect, float Level)
{
	Super::PreAttack_Implementation(Effect, Level);
}

AActor* AMKPlayer::GetTargetActor_Implementation() const
{
	return TargetEnemy.Get();
}

void AMKPlayer::EquipItem_Implementation(const FMKInventoryItemSpec& ItemData)
{

	switch (ItemData.ItemData.Panel)
	{
	case EInventoryPanel::Weapon:
		CombatComponent->SwapWeapon(FName(*(ItemData.ItemData.ItemName.ToString())));
		break;
	case EInventoryPanel::Helmet:
		if (ItemData.SK_Mesh)
		{
			Helmet->SetSkeletalMeshAsset(ItemData.SK_Mesh.Get());
		}
		break;
	case EInventoryPanel::Gloves:
		if (ItemData.SK_Mesh)
		{
			Gloves->SetSkeletalMeshAsset(ItemData.SK_Mesh.Get());
		}
		break;
	case EInventoryPanel::Suit:
		if (ItemData.SK_Mesh)
		{
			Suit->SetSkeletalMeshAsset(ItemData.SK_Mesh.Get());
		}
		break;
	case EInventoryPanel::Shoes:
		if (ItemData.SK_Mesh)
		{
			Shoes->SetSkeletalMeshAsset(ItemData.SK_Mesh.Get());
		}
		break;
	case EInventoryPanel::Gourd:
		break;
	case EInventoryPanel::HonBaek:
		break;
	case EInventoryPanel::Accessory:
		break;
	case EInventoryPanel::Consumable:
		break;
	case EInventoryPanel::Items:
		break;
	}
}

bool AMKPlayer::IsLockingOn() const
{
	return IsValid(PlayerCameraComponent->GetCameraLockOnTargetObject()->GetTargetActor());
}

void AMKPlayer::Transform()
{}

void AMKPlayer::ToggleHidden(bool bShow)
{
	SetActorHiddenInGame(bShow);
	SetActorEnableCollision(!bShow);

	TArray<AActor*> ChildActors;

	GetAttachedActors(ChildActors);
	for (AActor* ChildActor : ChildActors)
	{
		if (ChildActor)
		{
			ChildActor->SetActorHiddenInGame(bShow);
			ChildActor->SetActorEnableCollision(!bShow);
		}
	}
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
