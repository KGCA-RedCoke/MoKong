// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Actor/MKAbilityPlayerController.h"
#include "MKPlayerController.generated.h"

class UInventorySystemComponent;
class UPlayerHUDWidget;
/**
 * 
 */
UCLASS()
class MOKONG_API AMKPlayerController : public AMKAbilityPlayerController
{
	GENERATED_BODY()

public:
	AMKPlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void ResetState();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, Category= "Mokong|Player|Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerHUDWidget> PlayerAttributeWidget;

	UPROPERTY(BlueprintReadWrite, Category= "Mokong|Player|Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventorySystemComponent> PlayerInventory;
};
