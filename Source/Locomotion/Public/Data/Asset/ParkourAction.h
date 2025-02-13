// copyright Jacob Lim

#pragma once

#include "CoreMinimal.h"
#include "Data/AnimDataStruct.h"
#include "Engine/DataAsset.h"
#include "ParkourAction.generated.h"

/**
 * 
 */
UCLASS()
class LOCOMOTION_API UParkourAction : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bEnabled = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EParkourType ActionType = EParkourType::Default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EParkourWallHitResult WallHitType = EParkourWallHitResult::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ReachDistance = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FInt32Range CatchHeightDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FParkourRangeSetup RangeSetup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FParkourAnim> ParkourAnims;
};
