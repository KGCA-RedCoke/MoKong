// copyright Jacob Lim


#include "AnimNotify/FootStepAnimNotify.h"

#include "Component/FootStepSFXComponent.h"

void UFootStepAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (UFootStepSFXComponent* FootStepComponent = Cast<UFootStepSFXComponent>(MeshComp->GetOwner()->
		GetComponentByClass(
			UFootStepSFXComponent::StaticClass())))
	{
		FootStepComponent->SetAnimation(Animation);
		FootStepComponent->FootStep();
	}
}
