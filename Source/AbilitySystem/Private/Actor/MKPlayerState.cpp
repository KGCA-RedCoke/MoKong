// CopyRight KGCA - Team RedCoke


#include "Actor/MKPlayerState.h"

#include "Ability/MKAbilitySystemComponent.h"

AMKPlayerState::AMKPlayerState()
{
	SetNetUpdateFrequency(100.0f);

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
