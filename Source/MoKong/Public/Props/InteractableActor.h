// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

class UCommonInteractionWidget;
class AMKPlayer;
class UWidgetComponent;

UENUM(BlueprintType)
enum class EInteractionItemType : uint8
{
	Box_1,
	Box_2,
	Box_3,
	Box_4,
	Box_5,
	Box_6,
	Box_7,
	Box_8,
	Box_Jiasi,
	Box_Coffin1,
	Box_Coffin2,
	Shrine
};

/**
 * (상자) 상호작용할 수 있는 액터 
 */
UCLASS()
class MOKONG_API AInteractableActor : public AActor
{
	GENERATED_BODY()

public:
	AInteractableActor();

protected:
	virtual void BeginPlay() override;

	// protected:
	// 	UFUNCTION()
	// 	void OnEnterInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	// 							int32                OtherBodyIndex, bool         bFromSweep, const FHitResult& SweepResult);
	// 	UFUNCTION()
	// 	void OnEnterOuterSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	// 							int32                OtherBodyIndex, bool         bFromSweep, const FHitResult& SweepResult);
	// 	UFUNCTION()
	// 	void OnOutInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	// 						  int32                OtherBodyIndex);
	// 	UFUNCTION()
	// 	void OnOutOuterSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	// 						  int32                OtherBodyIndex);

	void Interact();

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	EInteractionItemType InteractionItemType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> InteractionWidgetComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> InnerInteractionSphere;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> OuterInteractionSphere;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bInteractionFinished;
};
