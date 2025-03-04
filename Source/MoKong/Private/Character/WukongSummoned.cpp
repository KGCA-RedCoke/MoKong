// CopyRight KGCA - Team RedCoke


#include "Character/WukongSummoned.h"


// Sets default values
AWukongSummoned::AWukongSummoned()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWukongSummoned::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWukongSummoned::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWukongSummoned::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

