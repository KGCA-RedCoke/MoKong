// CopyRight KGCA - Team RedCoke


#include "Actor/MKAbilityPlayer.h"

#include "AbilitySystemGlobals.h"
#include "Ability/MKAbilitySystemComponent.h"
#include "CombatSystem/Components/CombatSystemComp.h"
#include "GameFramework/PlayerState.h"
#include "Input/MKEnhancedInputComponent.h"


// Sets default values
AMKAbilityPlayer::AMKAbilityPlayer()
{}

void AMKAbilityPlayer::InitializeAbilitySystem()
{
	UE_LOG(LogTemp, Warning, TEXT( "Init Ability." ));
	AbilitySystemComponent = Cast<
		UMKAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetPlayerState()));

	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->InitializeAbilitySystemData(AbilitySystemInitializationData, GetPlayerState(), this);

	PostInitializeAbilitySystem();
}

void AMKAbilityPlayer::PossessedBy(AController* NewController)
{
	ACharacter::PossessedBy(NewController);

	InitializeAbilitySystem();

}

void AMKAbilityPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeAbilitySystem();
}

// Called when the game starts or when spawned
void AMKAbilityPlayer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMKAbilityPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMKAbilityPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!InputConfig)
	{
		return;
	}

	UMKEnhancedInputComponent* MoKongInputComponent = Cast<UMKEnhancedInputComponent>(PlayerInputComponent);
	check(MoKongInputComponent);

	TArray<uint32> BindHandles;
	MoKongInputComponent->BindAbilityActions(InputConfig,
											 this,
											 &AMKAbilityPlayer::InputAbilityInputTagPressed,
											 &AMKAbilityPlayer::InputAbilityInputTagReleased,
											 BindHandles);

	if (AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr, this);
	}
}

void AMKAbilityPlayer::InputAbilityInputTagPressed(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void AMKAbilityPlayer::InputAbilityInputTagReleased(FGameplayTag InputTag)
{
	AbilitySystemComponent->AbilityInputTagReleased(InputTag);
}
