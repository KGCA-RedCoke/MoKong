// CopyRight KGCA - Team RedCoke


#include "Actor/MKPlayerState.h"

#include "Ability/MKAbilitySystemComponent.h"

AMKPlayerState::AMKPlayerState()
{
	// If the NetUpdateFrequency is too low, there will be a delay on Ability activation / Effect application on the client.
	SetNetUpdateFrequency(100.0f);

	// Create the Ability System Component sub-object.
	AbilitySystemComponent = CreateDefaultSubobject<UMKAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void AMKPlayerState::SwapAbilityActorInfo(AActor* NewOwner, AActor* NewAvatar)
{
	AbilitySystemComponent->InitAbilityActorInfo(NewOwner, NewAvatar);
}

UAbilitySystemComponent* AMKPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
