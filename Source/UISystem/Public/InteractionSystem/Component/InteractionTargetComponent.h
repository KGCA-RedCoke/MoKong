// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionSystem/InteractionTypes.h"
#include "InteractionTargetComponent.generated.h"


class USphereComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionBegin, AActor*, Actor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionEnd,
											 const EInteractionResult,
											 InteractionResult,
											 const AActor*,
											 Actor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UISYSTEM_API UInteractionTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractionTargetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float                        DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

protected:
	void InitializeNotifyZones();
	void InitializeHighlightComponents();

	//~~ Begin Sphere Overlap Event
	UFUNCTION()
	void OnInnerZoneBeginOverlap(UPrimitiveComponent* OverlappedComp,
								 AActor*              OtherActor, UPrimitiveComponent* OtherComp,
								 int32                OtherBodyIndex, bool             bFromSweep,
								 const FHitResult&    SweepResult);
	UFUNCTION()
	void OnInnerZoneEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32        OtherBodyIndex);

	UFUNCTION()
	void OnOuterZoneBeginOverlap(UPrimitiveComponent* OverlappedComp,
								 AActor*              OtherActor, UPrimitiveComponent* OtherComp,
								 int32                OtherBodyIndex, bool             bFromSweep,
								 const FHitResult&    SweepResult);
	UFUNCTION()
	void OnOuterZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32             OtherBodyIndex);
	//~~ End Sphere Overlap Event

public:
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractionBegin OnInteractionBegin;
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractionEnd OnInteractionEnd;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Debug", DisplayName= "디버깅모드 활성화")
	bool bDebugMode;

	/* 상호작용 할 오브젝트 레퍼런스 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Actor")
	AActor* TargetRef;

	/* 화면에 보여지는 최소 범위 (상호작용 X) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Data")
	USphereComponent* OuterNotifyZone;

	/* 실제 상호작용 가능 범위 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Data")
	USphereComponent* InnerNotifyZone;

	/* 상호작용 진행률 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Data")
	float ScreenRadiusProgress;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Data")
	FMargin WidgetMargin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Data")
	TArray<TObjectPtr<UPrimitiveComponent>> HighlightedComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Data")
	TArray<TObjectPtr<AController>> AssignedInteractors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Data")
	TObjectPtr<AController> AssignedInteractor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Data")
	TObjectPtr<USceneComponent> MarkerTargetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Data")
	bool bInteractionEnabled = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Data")
	float LastInteractionTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings|Marker")
	bool bDisplayProgress;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings|Marker")
	bool bDisplayTriangle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings|Marker")
	bool bFlashPOIIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings|Marker")
	bool bFlashTargetIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings|Marker")
	bool bRotateInteractionIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings|Marker")
	FString MarkerTargetName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings|Marker")
	FVector MarkerOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings|Marker")
	FLinearColor TargetIcon_Color;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings|Marker")
	FLinearColor POIIcon_Color;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings|Marker")
	FLinearColor Background_Color;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings|Marker")
	FLinearColor Foreground_Color;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings|Marker")
	FInteractionIcon Target_Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings|Marker")
	FInteractionIcon POI_Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings|Marker")
	FInteractionIcon Background_Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings|Marker")
	FInteractionIcon Foreground_Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings|Highlight")
	FLinearColor Highlight_Color;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings|Highlight")
	TSet<FString> ComponentsToHighlights;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings")
	bool bCooldownEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings")
	int RepeatCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings")
	float RepeatCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings")
	float TimeToHold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings")
	float InnerZoneRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings")
	float OuterZoneExtent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings")
	FString AttachingComponentName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings")
	EInteractionType InteractionType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Settings")
	TMap<EInteractionState, FText> InteractionText;
};
