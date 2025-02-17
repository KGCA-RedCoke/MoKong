// CopyRight KGCA - Team RedCoke


#include "Actor/MKAbilityPlayerController.h"

#include "Ability/MKAbilitySystemComponent.h"
#include "Actor/MKPlayerState.h"

AMKPlayerState* AMKAbilityPlayerController::GetPTPlayerState() const
{
	return CastChecked<AMKPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UMKAbilitySystemComponent* AMKAbilityPlayerController::GetMKAbilitySystemComponent() const
{
	const AMKPlayerState* PS = GetPTPlayerState();
	return CastChecked<UMKAbilitySystemComponent>(PS->GetAbilitySystemComponent());
}

void AMKAbilityPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void AMKAbilityPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UMKAbilitySystemComponent* ASC = GetMKAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}
