// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MKTargetType.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class ABILITYSYSTEM_API UMKTargetType : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(ACharacter*         TargetingCharacter, AActor*     TargetingActor, FGameplayEventData EventData,
					TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActor) const;

};

UCLASS(NotBlueprintable)
class UTargetType_UseOwner : public UMKTargetType
{
	GENERATED_BODY()

public:
	virtual void GetTargets_Implementation(ACharacter*        TargetingCharacter, AActor*    TargetingActor,
										   FGameplayEventData EventData, TArray<FHitResult>& OutHitResults,
										   TArray<AActor*>&   OutActor) const override;
};

UCLASS(NotBlueprintable)
class UTargetType_UseEventData : public UMKTargetType
{
	GENERATED_BODY()

public:
	virtual void GetTargets_Implementation(ACharacter*        TargetingCharacter, AActor*    TargetingActor,
										   FGameplayEventData EventData, TArray<FHitResult>& OutHitResults,
										   TArray<AActor*>&   OutActor) const override;
};
