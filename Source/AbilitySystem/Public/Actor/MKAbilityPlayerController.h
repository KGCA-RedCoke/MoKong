// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MKAbilityPlayerController.generated.h"

class UMKAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API AMKAbilityPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "InputSystem|PlayerController")
	AMKPlayerState* GetPTPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "InputSystem|ISAbilitySystemComponent")
	UMKAbilitySystemComponent* GetPTAbilitySystemComponent() const;

protected:
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
};
