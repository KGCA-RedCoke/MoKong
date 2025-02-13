// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MoKongTypes.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Stable UMETA(DisplayName = "안정"),
	Battle UMETA(DisplayName = "전투"),
	Action UMETA(DisplayName = "행동"),
	Dead UMETA(DisplayName = "죽음")
};
