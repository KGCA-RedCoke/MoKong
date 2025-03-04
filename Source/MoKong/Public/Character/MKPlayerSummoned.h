// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Actor/MKAbilityCharacter.h"
#include "AI/Interface/EnemyAIInterface.h"
#include "MKPlayerSummoned.generated.h"

class UPoseableMeshComponent;

UCLASS()
class MOKONG_API AMKPlayerSummoned : public AMKAbilityCharacter
{
	GENERATED_BODY()

public:
	AMKPlayerSummoned();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void ReplicateMeshFromPlayer();

private:


	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPoseableMeshComponent> PoseableMesh;
};
