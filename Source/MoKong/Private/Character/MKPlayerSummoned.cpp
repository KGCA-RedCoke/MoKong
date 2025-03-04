// CopyRight KGCA - Team RedCoke


#include "Character/MKPlayerSummoned.h"

#include "Components/PoseableMeshComponent.h"


// Sets default values
AMKPlayerSummoned::AMKPlayerSummoned()
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
void AMKPlayerSummoned::BeginPlay()
{
	Super::BeginPlay();
}

void AMKPlayerSummoned::ReplicateMeshFromPlayer()
{
	// PoseableMesh->CopyPoseFromSkeletalComponent()
}
