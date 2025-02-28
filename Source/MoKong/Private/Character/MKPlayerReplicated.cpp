// CopyRight KGCA - Team RedCoke


#include "Character/MKPlayerReplicated.h"

#include "Components/PoseableMeshComponent.h"


// Sets default values
AMKPlayerReplicated::AMKPlayerReplicated()
{
	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));
	PoseableMesh->SetupAttachment(GetRootComponent());

	Head->SetupAttachment(PoseableMesh);
	Helmet->SetupAttachment(PoseableMesh);
	Suit->SetupAttachment(PoseableMesh);
	Shoes->SetupAttachment(PoseableMesh);
	Gloves->SetupAttachment(PoseableMesh);
}

// Called when the game starts or when spawned
void AMKPlayerReplicated::BeginPlay()
{
	Super::BeginPlay();
}

void AMKPlayerReplicated::ReplicateMeshFromPlayer()
{
	// PoseableMesh->CopyPoseFromSkeletalComponent()
}

