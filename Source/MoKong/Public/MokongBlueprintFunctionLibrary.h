// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MokongBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MOKONG_API UMokongBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static EHitReactDirection GetHitDirection(AActor* Target , const FVector& ImpactPoint);
};
