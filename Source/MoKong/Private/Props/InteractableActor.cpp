// CopyRight KGCA - Team RedCoke


#include "Props/InteractableActor.h"

#include "Character/MKPlayer.h"
#include "Components/WidgetComponent.h"
#include "UI/Interaction/CommonInteractionWidget.h"


// Sets default values
AInteractableActor::AInteractableActor()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodySkeletalMeshComponent"));
	SetRootComponent(SkeletalMeshComponent.Get());

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InteractionWidgetComponent->SetupAttachment(GetRootComponent());
	InteractionWidgetComponent->SetVisibility(false);

	InnerInteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InnerInteractionSphere"));
	InnerInteractionSphere->SetupAttachment(GetRootComponent());
	OuterInteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OuterInteractionSphere"));
	OuterInteractionSphere->SetupAttachment(GetRootComponent());
	//
	// InnerInteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnEnterInnerSphere);
	// OuterInteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnEnterOuterSphere);
	// InnerInteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnOutInnerSphere);
	// OuterInteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnOutOuterSphere);
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();

}

// void AInteractableActor::OnEnterInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor*  OtherActor,
// 											UPrimitiveComponent* OtherComp, int32              OtherBodyIndex,
// 											bool                 bFromSweep, const FHitResult& SweepResult)
// {
// 	// Switch Icon
// }
//
// void AInteractableActor::OnEnterOuterSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 											UPrimitiveComponent* OtherComp,
// 											int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	InteractionWidgetComponent->SetVisibility(true);
// }
//
// void AInteractableActor::OnOutInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 										  UPrimitiveComponent* OtherComp, int32             OtherBodyIndex)
// {}
//
// void AInteractableActor::OnOutOuterSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 										  UPrimitiveComponent* OtherComp, int32             OtherBodyIndex)
// {
// 	InteractionWidgetComponent->SetVisibility(false);
// }

void AInteractableActor::Interact()
{


	bInteractionFinished = true;
}
