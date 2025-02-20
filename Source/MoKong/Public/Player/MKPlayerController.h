// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Actor/MKAbilityPlayerController.h"
#include "MKPlayerController.generated.h"

class UPlayerHUDWidget;
/**
 * 
 */
UCLASS()
class MOKONG_API AMKPlayerController : public AMKAbilityPlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintReadWrite, Category= "Mokong|Player|Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerHUDWidget> PlayerAttributeWidget;
};
