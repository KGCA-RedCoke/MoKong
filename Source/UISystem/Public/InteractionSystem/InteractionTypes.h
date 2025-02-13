// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InteractionTypes.generated.h"


UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	None,
	Tap,
	Hold,
	Repeat
};

UENUM(BlueprintType)
enum class EInteractionState : uint8
{
	None,
	Waiting,
	Interacting,
	Done
};

UENUM(BlueprintType)
enum class EInteractionResult : uint8
{
	None,
	Completed,
	Canceled,
};

UENUM(BlueprintType, Meta = (ToolTip = "상호작용이 끝난 후의 처리 방법"))
enum class EInteractionFinishMethod : uint8
{
	None,
	DestroyOnComplete UMETA(ToolTip = "상호작용이 끝난 후 제거"),
	DestroyOnCancel UMETA(ToolTip = "상호작용이 취소되면 제거"),
	DestroyOnCompleteOrCancel UMETA(ToolTip = "상호작용이 끝나거나 취소되면 제거"),
	ReactivateOnComplete UMETA(ToolTip = "상호작용이 끝나면 다시 활성화"),
	ReactivateOnCancel UMETA(ToolTip = "상호작용이 취소되면 다시 활성화"),
	ReactivateOnCompleteOrCancel UMETA(ToolTip = "상호작용이 끝나거나 취소되면 다시 활성화"),
	ReactivateOnCancelDestroyOnComplete UMETA(ToolTip = "상호작용이 취소되면 다시 활성화, 끝나면 제거"),
	ReactivateOnCancelDeactivateOnComplete UMETA(ToolTip = "상호작용이 취소되면 다시 활성화, 끝나면 비활성화"),
	ReactivateOnCompleteDestroyOnCancel UMETA(ToolTip = "상호작용이 끝나면 다시 활성화, 취소되면 제거"),
	ReactivateOnCompleteDeactivateOnCancel UMETA(ToolTip = "상호작용이 끝나면 다시 활성화, 취소되면 비활성화"),
	DeactivateOnComplete UMETA(ToolTip = "상호작용이 끝나면 비활성화"),
	DeactivateOnCancel UMETA(ToolTip = "상호작용이 취소되면 비활성화"),
	DeactivateOnCompleteOrCancel UMETA(ToolTip = "상호작용이 끝나거나 취소되면 비활성화"),
	DeactivateOnCanceledDestroyOnComplete UMETA(ToolTip = "상호작용이 취소되면 비활성화, 끝나면 제거"),
	DeactivateOnCanceledReactivateAfterDurationOnComplete UMETA(ToolTip = "상호작용이 취소되면 비활성화, 끝나면 지정된 시간 후 다시 활성화"),
	DeactivateOnCompletedDestroyOnCancel UMETA(ToolTip = "상호작용이 끝나면 비활성화, 취소되면 제거"),
	DeactivateOnCompletedReactivateOnCancel UMETA(ToolTip = "상호작용이 끝나면 비활성화, 취소되면 다시 활성화")
};

USTRUCT(BlueprintType)
struct FInteractionIcon
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseMaterialAsTexture = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterialInstance> Material;
};

USTRUCT(BlueprintType)
struct FLockOnWidgetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowLockOn = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableRotation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Size{20, 20};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color = FColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ZAxisHeight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETextFlowDirection RotationOrientation = ETextFlowDirection::LeftToRight;
};
