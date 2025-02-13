// copyright Jacob Lim


#include "Component/LocomotionComponent_Base.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


void ULocomotionComponent_Base::HandleInput(const FVector2D& InputValue)
{
	CurrentInputDirection =
		UKismetMathLibrary::Quat_RotateVector(
			FRotator(0, GetCharacter()->GetControlRotation().Yaw, 0).Quaternion(),
			FVector(InputValue.X, InputValue.Y, 0.f)).GetSafeNormal();

	if (!CurrentInputDirection.IsZero())
	{
		LastInputDirection = CurrentInputDirection;
	}
}

void ULocomotionComponent_Base::BeginPlay()
{
	Super::BeginPlay();

	// 캐릭터 정보  가져오기
	CharacterData.Character = Cast<ACharacter>(GetOwner());
	check(CharacterData.Character)

	CharacterData.CapsuleCollision = CharacterData.Character->GetCapsuleComponent();
	CharacterData.SkeletalMeshComponent = CharacterData.Character->GetMesh();
	CharacterData.MovementComponent = CharacterData.Character->GetCharacterMovement();

	float OutRadius, OutHalfRadius;
	CharacterData.CapsuleCollision->GetScaledCapsuleSize(OutRadius, OutHalfRadius);
	CharacterData.DefaultCapsuleSize = FVector2D(OutRadius, OutHalfRadius);

	// 이벤트 델리게이트 등록
	CharacterData.Character->LandedDelegate.AddDynamic(this, &ThisClass::OnCharacterLanded);
	CharacterData.Character->MovementModeChangedDelegate.AddDynamic(this, &ThisClass::OnMovementModeChanged);
	CharacterData.Character->OnCharacterMovementUpdated.AddDynamic(this, &ThisClass::OnCharacterMovementUpdated);
	CharacterData.MovementComponent->bNotifyApex = true;
	CharacterData.Character->OnReachedJumpApex.AddDynamic(this, &ThisClass::OnReachedApex);

	CharacterData.SkeletalMeshComponent->OnAnimInitialized.AddDynamic(this, &ThisClass::OnAnimInstanceInitialized);
	OnAnimInstanceInitialized();

	if (CharacterData.Character->IsLocallyControlled())
	{
		CharacterData.AnimationInstance->OnMontageBlendingOut.AddDynamic(this, &ThisClass::OnMontageBlendOut);
	}

	// Skeletal Mesh보다 이 컴포넌트를 먼저 실행
	CharacterData.SkeletalMeshComponent->AddTickPrerequisiteComponent(this);
	// MovementComponent보다 이 컴포넌트를 먼저 실행
	CharacterData.MovementComponent->AddTickPrerequisiteComponent(this);
	// 캐릭터보다 SkeletalMesh를 먼저 실행
	CharacterData.SkeletalMeshComponent->AddTickPrerequisiteActor(CharacterData.Character);
}

void ULocomotionComponent_Base::OnAnimInstanceInitialized()
{
	// 애님 인스턴스 초기화
	CharacterData.AnimationInstance = CharacterData.SkeletalMeshComponent->GetAnimInstance();
}

void ULocomotionComponent_Base::OnCharacterMovementUpdated(float DeltaSeconds, FVector OldLocation, FVector OldVelocity)
{
	// 하위 클래스에서 사용될 가속도와 가속도방향
	CurrentAcceleration = GetMovementComponent()->GetCurrentAcceleration();

	if (!CurrentAcceleration.IsZero())
	{
		LastAccelerationDirection = CurrentAcceleration.GetSafeNormal();
	}
}

void ULocomotionComponent_Base::OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode,
                                                      uint8 PreviousCustomMode)
{
}

void ULocomotionComponent_Base::OnCharacterLanded(const FHitResult& Hit)
{
	CharacterData.MovementComponent->bNotifyApex = true;
}

void ULocomotionComponent_Base::OnMontageBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
}

void ULocomotionComponent_Base::OnReachedApex()
{
}

#pragma region Getter
ACharacter* ULocomotionComponent_Base::GetCharacter() const
{
	return CharacterData.Character;
}

UCapsuleComponent* ULocomotionComponent_Base::GetCapsuleComponent() const
{
	return CharacterData.CapsuleCollision;
}

USkeletalMeshComponent* ULocomotionComponent_Base::GetSkeletalMeshComponent() const
{
	return CharacterData.SkeletalMeshComponent;
}

UCharacterMovementComponent* ULocomotionComponent_Base::GetMovementComponent() const
{
	return CharacterData.MovementComponent;
}

UAnimInstance* ULocomotionComponent_Base::GetAnimInstance() const
{
	return CharacterData.AnimationInstance;
}

void ULocomotionComponent_Base::GetCapsuleSize(const bool bScaled, float& OutRadius, float& OutHalfRadius) const
{
	if (bScaled)
		GetCapsuleComponent()->GetScaledCapsuleSize(OutRadius, OutHalfRadius);
	else
	{
		OutRadius = CharacterData.DefaultCapsuleSize.Y;
		OutHalfRadius = CharacterData.DefaultCapsuleSize.X;
	}
}
#pragma endregion

bool ULocomotionComponent_Base::Wait(float& TimeReference, const float RetriggerTime) const
{
	if (UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld()) >= TimeReference + RetriggerTime)
	{
		TimeReference = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
	}
	return true;
}
