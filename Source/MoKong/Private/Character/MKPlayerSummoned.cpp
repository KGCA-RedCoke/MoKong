// CopyRight KGCA - Team RedCoke


#include "Character/MKPlayerSummoned.h"

#include "AI/Interface/EnemyAIInterface.h"
#include "CombatSystem/Components/CombatSystemComp.h"
#include "Components/PoseableMeshComponent.h"


AMKPlayerSummoned::AMKPlayerSummoned()
{
	Helmet->SetSkeletalMeshAsset(HelmetMesh.Get());
	Suit->SetSkeletalMeshAsset(SuitMesh.Get());
	Gloves->SetSkeletalMeshAsset(GlovesMesh.Get());
	Shoes->SetSkeletalMeshAsset(ShoesMesh.Get());
}

void AMKPlayerSummoned::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeTimeLevel);

	CombatComponent->InitializeCombatSystem(GetMesh());

	PlayAnimMontage(SummonedMontage.LoadSynchronous(), 1.f);

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle,
										   [&](){
											   PlayAnimMontage(DisappearMontage.LoadSynchronous(), 1.f);
										   },
										   LifeTimeLevel * 0.8f,
										   false);
}

void AMKPlayerSummoned::ReplicateMeshFromPlayer()
{
	// PoseableMesh->CopyPoseFromSkeletalComponent()
}
