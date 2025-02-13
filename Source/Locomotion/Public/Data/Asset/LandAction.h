// copyright Jacob Lim

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LandAction.generated.h"

/**
 * 
 */
UCLASS()
class LOCOMOTION_API ULandAction : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bEnabled = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FInt32Range SpeedSetup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FInt32Range ApexDistanceSetup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UAnimMontage*> LandAnims;
};
