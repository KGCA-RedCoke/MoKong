// CopyRIght JacobLim

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Data/AnimDataStruct.h"
#include "HumanMovement.generated.h"

/**
 * 인간 형태의 캐릭터의 움직임을 담당하는 애니메이션 인스턴스
 */
UCLASS()
class LOCOMOTION_API UHumanMovement : public UAnimInstance
{
	GENERATED_BODY()

public:
	
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Movement|Overlay")
	float ArmLeftLocal = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Movement|Overlay")
	float ArmRightLocal = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement|Data|State")
	ELocomotionStances LastStance;
	UPROPERTY(BlueprintReadOnly, Category = "Movement|Data|State")
	ELocomotionStances CurrentStance;
	UPROPERTY(BlueprintReadOnly, Category = "Movement|Data|State")
	EMovementType LocomotionType;
	UPROPERTY(BlueprintReadOnly, Category = "Movement|Data|State")
	ECharacterState CharacterState;
	UPROPERTY(BlueprintReadOnly, Category = "Movement|Data|State")
	EOverlayStates OverlayState;
	UPROPERTY(BlueprintReadOnly, Category = "Movement|Data|State")
	ERotationMode RotationMode;


};
