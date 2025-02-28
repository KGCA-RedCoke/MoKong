// CopyRight KGCA - Team RedCoke


#include "Actor/MKAbilityNPC.h"

#include "Ability/MKAbilitySystemComponent.h"


// Sets default values
AMKAbilityNPC::AMKAbilityNPC()
{
	// Non Player Character는 캐릭터 자체에 Ability System Component를 가지고 있는다.
	AbilitySystemComponent = CreateDefaultSubobject<UMKAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

}

// Called when the game starts or when spawned
void AMKAbilityNPC::BeginPlay()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitializeAbilitySystemData(AbilitySystemInitializationData, this, this);

		PostInitializeAbilitySystem();
	}

	ACharacter::BeginPlay();

}
