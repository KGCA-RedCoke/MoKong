// CopyRight KGCA - Team RedCoke


#include "Character/MKPlayerReplicated.h"


// Sets default values
AMKPlayerReplicated::AMKPlayerReplicated()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMKPlayerReplicated::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMKPlayerReplicated::ReplicateMeshFromPlayer()
{
		
}

