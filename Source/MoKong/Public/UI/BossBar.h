// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "HealthBar/CatchUpBarBase.h"
#include "BossBar.generated.h"

/**
 * 
 */
UCLASS()
class MOKONG_API UBossBar : public UCatchUpBarBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BossBar")
	FName BossName;
};
