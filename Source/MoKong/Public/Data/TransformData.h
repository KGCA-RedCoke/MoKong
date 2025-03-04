// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TransformData.generated.h"

enum class EWeaponType : uint8;
/**
 * 오공 -> 변형 데이터
 */
UCLASS()
class MOKONG_API UTransformData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName TransformName;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UAnimInstance> AnimationInstance;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USkeletalMesh> AvatarMesh;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USkeletalMesh> AdditionalMesh;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
};
