// copyright Jacob Lim

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/AnimDataStruct.h"
#include "LocomotionComponent_Base.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class LOCOMOTION_API ULocomotionComponent_Base : public UActorComponent
{
	GENERATED_BODY()

public:
	/* 캐릭터 시선의 Yaw를 쿼터니언 변환 후 입력축을 회전 (어디곳으로 가야할지 방향 판별)*/
	UFUNCTION(BlueprintCallable)
	void HandleInput(const FVector2D& InputValue);

protected:
	virtual void BeginPlay() override;

#pragma region Callback
	UFUNCTION()
	virtual void OnAnimInstanceInitialized();
	UFUNCTION()
	virtual void OnCharacterMovementUpdated(float DeltaSeconds, FVector OldLocation, FVector OldVelocity);
	UFUNCTION()
	virtual void OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode);
	UFUNCTION()
	virtual void OnCharacterLanded(const FHitResult& Hit);
	UFUNCTION()
	virtual void OnMontageBlendOut(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	virtual void OnReachedApex();
#pragma endregion

#pragma region Getter
	UFUNCTION(BlueprintCallable)
	ACharacter* GetCharacter() const;
	UFUNCTION(BlueprintCallable)
	UCapsuleComponent* GetCapsuleComponent() const;
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetSkeletalMeshComponent() const;
	UFUNCTION(BlueprintCallable)
	UCharacterMovementComponent* GetMovementComponent() const;
	UFUNCTION(BlueprintCallable)
	UAnimInstance* GetAnimInstance() const;
	UFUNCTION(BlueprintCallable)
	void GetCapsuleSize(const bool bScaled, float& OutRadius, float& OutHalfRadius) const;
#pragma endregion

	bool Wait(float& TimeReference, const float RetriggerTime) const;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent_Base|Character")
	FCharacterData CharacterData;

	/* 현재 가속도 */
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent_Base|Velocity")
	FVector CurrentAcceleration;

	/* 최근의 가속도 방향 */
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent_Base|Velocity")
	FVector LastAccelerationDirection;

	/* 현재 입력키 방향 */
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent_Base|Input")
	FVector CurrentInputDirection;

	/* 최근의 입력키 방향 */
	UPROPERTY(BlueprintReadWrite, Category = "LocomotionComponent_Base|Input")
	FVector LastInputDirection;
};
