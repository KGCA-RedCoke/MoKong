// copyright Jacob Lim

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FootStepAnimNotify.generated.h"

/**
 * 발이 지면에 닿을 때 이 노티파이 실행 
 */
UCLASS()
class FOOTSFX_API UFootStepAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;
};
