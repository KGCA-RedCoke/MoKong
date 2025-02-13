// CopyRight KGCA - Team RedCoke


#include "InteractionSystem/Component/InteractionTargetComponent.h"

#include "Components/SphereComponent.h"


// Sets default values for this component's properties
UInteractionTargetComponent::UInteractionTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractionTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	TargetRef = GetOwner();
	check(TargetRef);

	InitializeNotifyZones();
	InitializeHighlightComponents();
}


// Called every frame
void UInteractionTargetComponent::TickComponent(float                        DeltaTime, ELevelTick TickType,
												FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractionTargetComponent::InitializeNotifyZones()
{
	// 액터의 모든 씬 컴포넌트를 순회할 것임
	TArray<USceneComponent*> SceneComponents;
	GetOwner()->GetComponents<USceneComponent>(SceneComponents);

	USceneComponent* ComponentToAttach = nullptr;

	// 액터의 모든 씬 컴포넌트를 순회하며 이름이 일치하는 컴포넌트(타겟이 될)를 찾아서 저장
	for (USceneComponent* SceneComponent : SceneComponents)
	{
		if (SceneComponent->GetName().Equals(AttachingComponentName))
		{
			ComponentToAttach = SceneComponent;
			break;
		}
	}

	// Inner, Outer 노티파이 존을 생성하고 AttachToComponent로 ComponentToAttach에 붙임
	InnerNotifyZone = NewObject<USphereComponent>(
												  GetOwner(),
												  TEXT("InnerNotifyRadius"));

	OuterNotifyZone = NewObject<USphereComponent>(
												  GetOwner(),
												  TEXT("OuterNotifyRadius"));

	check(InnerNotifyZone);
	check(OuterNotifyZone);

	InnerNotifyZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	InnerNotifyZone->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	OuterNotifyZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	OuterNotifyZone->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);

	InnerNotifyZone->InitSphereRadius(InnerZoneRadius);
	OuterNotifyZone->InitSphereRadius(InnerZoneRadius + FMath::Max(OuterZoneExtent, 10.f));

	InnerNotifyZone->SetHiddenInGame(!bDebugMode);
	OuterNotifyZone->SetHiddenInGame(!bDebugMode);

	GetOwner()->AddComponent(TEXT("InnerNotify"), false, FTransform::Identity, InnerNotifyZone);

	if (ComponentToAttach)
	{
		InnerNotifyZone->SetupAttachment(ComponentToAttach);
		InnerNotifyZone->RegisterComponent();

		OuterNotifyZone->SetupAttachment(InnerNotifyZone);
		OuterNotifyZone->RegisterComponent();
	}


	InnerNotifyZone->OnComponentBeginOverlap.AddDynamic(this,
														&ThisClass::OnInnerZoneBeginOverlap);
	InnerNotifyZone->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnInnerZoneEndOverlap);

	OuterNotifyZone->OnComponentBeginOverlap.AddDynamic(this,
														&ThisClass::OnOuterZoneBeginOverlap);
	OuterNotifyZone->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnOuterZoneEndOverlap);
}

void UInteractionTargetComponent::InitializeHighlightComponents()
{
	USceneComponent* OwnerRootComp = GetOwner()->GetRootComponent();

	check(OwnerRootComp);

	if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(OwnerRootComp))
	{
		if (ComponentsToHighlights.Contains(PrimitiveComponent->GetName()))
		{
			HighlightedComponents.AddUnique(PrimitiveComponent);
		}
	}

	TArray<USceneComponent*> ChildComponents;
	OwnerRootComp->GetChildrenComponents(true, ChildComponents);

	for (USceneComponent* ChildComponent : ChildComponents)
	{
		if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(ChildComponent))
		{
			if (ComponentsToHighlights.Contains(PrimitiveComponent->GetName()))
			{
				HighlightedComponents.AddUnique(PrimitiveComponent);
			}
		}
	}
}

void UInteractionTargetComponent::OnInnerZoneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
														  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
														  bool                 bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == TargetRef)
		return;

	const APawn* Pawn = Cast<APawn>(OtherActor);
	check(Pawn)
	// if (AController* Controller = Pawn->GetController())
	// {
	// 	if (UInteractionComponent* InteractionComponent = UInteractionComponent::GetInteractionComponent(Controller))
	// 	{
	// 		InteractionComponent->OnInteractionTargetUpdated_Server(true, this);
	// 		AssignInteractor(Controller);
	// 	}
	// }
}

void UInteractionTargetComponent::OnInnerZoneEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
														UPrimitiveComponent* OtherComp, int32        OtherBodyIndex)
{
	if (OtherActor == TargetRef)
		return;

	const APawn* Pawn = Cast<APawn>(OtherActor);
	check(Pawn)
	// const AController* Controller = Pawn->GetController();
	//
	// if (Controller)
	// 	if (UInteractionComponent* InteractionComponent = UInteractionComponent::GetInteractionComponent(Controller))
	// 	{
	// 		InteractionComponent->OnInteractionTargetUpdated_Server(false, this);
	// 	}
}

void UInteractionTargetComponent::OnOuterZoneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
														  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
														  bool                 bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == TargetRef)
		return;

	const APawn* Pawn = Cast<APawn>(OtherActor);
	check(Pawn)
	// if (AController* Controller = Pawn->GetController())
	// {
	// 	if (UInteractionComponent* InteractionComponent = UInteractionComponent::GetInteractionComponent(Controller))
	// 	{
	// 		InteractionComponent->OnInteractionTargetUpdated_Server(true, this);
	// 		AssignInteractor(Controller);
	// 	}
	// }
}

void UInteractionTargetComponent::OnOuterZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
														UPrimitiveComponent* OtherComp, int32             OtherBodyIndex)
{
	if (OtherActor == TargetRef)
		return;

	const APawn* Pawn = Cast<APawn>(OtherActor);
	check(Pawn)

	// if (const AController* Controller = Pawn->GetController())
	// {
	// 	if (UInteractionComponent* InteractionComponent = UInteractionComponent::GetInteractionComponent(Controller))
	// 	{
	// 		InteractionComponent->OnPointOfInterestUpdated_Server(true, this);
	// 	}
	// }
}
