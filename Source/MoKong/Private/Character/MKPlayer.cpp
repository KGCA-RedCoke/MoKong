// CopyRight KGCA - Team RedCoke


#include "MoKong/Public/Character/MKPlayer.h"

#include "ATPCCameraComponent.h"
#include "ATPCCameraLockOnTargetObject.h"
#include "Character/MokongEnemy.h"
#include "Component/FootStepSFXComponent.h"

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
			if (!TargetEnemy)
				return;
			TargetEnemy->ShowLockOnWidget(true);
		}
		else
		{
			if (TargetEnemy)
			{
				TargetEnemy->ShowLockOnWidget(false);
				TargetEnemy = nullptr;
			}
		}
		break;
	case EATPCChangeTargetReason::LostByExitCameraMode:
	case EATPCChangeTargetReason::LostByDistance:
	case EATPCChangeTargetReason::LostByVisible:
	case EATPCChangeTargetReason::LostByPlayerInput:
		if (TargetEnemy)
		{
			TargetEnemy->ShowLockOnWidget(false);
			TargetEnemy = nullptr;
		}
		break;
	}
}
