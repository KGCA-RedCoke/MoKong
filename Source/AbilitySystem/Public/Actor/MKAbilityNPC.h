// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "MKAbilityCharacter.h"
#include "MKAbilityNPC.generated.h"

UCLASS(Blueprintable)
class ABILITYSYSTEM_API AMKAbilityNPC : public AMKAbilityCharacter
{
	GENERATED_BODY()

public:
	AMKAbilityNPC();

protected:
	virtual void BeginPlay() override;

};
