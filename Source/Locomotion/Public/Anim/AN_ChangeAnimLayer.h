// copyright Jacob Lim

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_ChangeAnimLayer.generated.h"

/**
 * 
 */
UCLASS()
class LOCOMOTION_API UAN_ChangeAnimLayer : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AnimLayer")
	TSubclassOf<UAnimInstance> LayerToLink;
};
