// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Actor/MKAbilityNPC.h"
#include "MokongEnemy.generated.h"

class UWidgetComponent;

UCLASS()
class MOKONG_API AMokongEnemy : public AMKAbilityNPC
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMokongEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void ShowLockOnWidget(bool bShow);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category= "LockOn")
	TObjectPtr<UWidgetComponent> LockOnWidgetComponent;

};
