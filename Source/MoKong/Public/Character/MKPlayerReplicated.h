// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Actor/MKAbilityCharacter.h"
#include "MKPlayerReplicated.generated.h"

UCLASS()
class MOKONG_API AMKPlayerReplicated : public AMKAbilityCharacter
{
	GENERATED_BODY()

public:
	AMKPlayerReplicated();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void ReplicateMeshFromPlayer();


};
