// copyright Jacob Lim


#include "Anim/AN_ChangeAnimLayer.h"


void UAN_ChangeAnimLayer::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	MeshComp->LinkAnimClassLayers(LayerToLink);
}
