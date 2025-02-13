// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "MKPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API AMKPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMKPlayerState();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Ability System")
	UAbilitySystemComponent* AbilitySystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
