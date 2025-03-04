// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "MoKongTypes.h"
#include "Engine/DataAsset.h"
#include "MeshPartsData.generated.h"

/**
 * 메시 파츠(세트) 데이터 에셋
 */
UCLASS()
class MOKONG_API UMeshPartsData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FMKItemSpec HeadSpec;

	UPROPERTY(EditAnywhere)
	FMKItemSpec HelmetSpec;

	UPROPERTY(EditAnywhere)
	FMKItemSpec SuitSpec;

	UPROPERTY(EditAnywhere)
	FMKItemSpec ShoesSpec;

	UPROPERTY(EditAnywhere)
	FMKItemSpec GlovesSpec;
};
