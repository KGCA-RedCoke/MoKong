// CopyRight KGCA - Team RedCoke


#include "Actor/MKAbilityCharacter.h"
#include "MotionWarpingComponent.h"
#include "NiagaraComponent.h"
#include "Ability/MKAbilitySystemComponent.h"
#include "AttributeSets/AttributeSet_Health.h"
#include "CombatSystem/Components/CombatSystemComp.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AMKAbilityCharacter::AMKAbilityCharacter()
{
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	CombatComponent        = CreateDefaultSubobject<UCombatSystem>(TEXT("CombatComponent"));

	//~~ Modular Parts
	Head   = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Helmet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Helmet"));
	Suit   = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Suit"));
	Shoes  = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoes"));
	Gloves = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gloves"));

	Head->SetupAttachment(GetMesh());
	Helmet->SetupAttachment(GetMesh());
	Suit->SetupAttachment(GetMesh());
	Shoes->SetupAttachment(GetMesh());
	Gloves->SetupAttachment(GetMesh());

	Head->SetLeaderPoseComponent(GetMesh());
	Helmet->SetLeaderPoseComponent(GetMesh());
	Suit->SetLeaderPoseComponent(GetMesh());
	Shoes->SetLeaderPoseComponent(GetMesh());
	Gloves->SetLeaderPoseComponent(GetMesh());
	//~~ Modular Parts
}

void AMKAbilityCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMKAbilityCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

UAbilitySystemComponent* AMKAbilityCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMKAbilitySystemComponent* AMKAbilityCharacter::GetMKAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCombatSystem* AMKAbilityCharacter::GetCombatComponent_Implementation()
{
	return CombatComponent->GetCombatComponent_Implementation();
}

FName AMKAbilityCharacter::GetSectionName_Implementation() const
{
	return CombatComponent->GetSectionName_Implementation();
}

bool AMKAbilityCharacter::IsComboWindowOpen_Implementation() const
{
	return CombatComponent->IsComboWindowOpen_Implementation();
}

bool AMKAbilityCharacter::IsNextAttackAvailable_Implementation() const
{
	return CombatComponent->IsNextAttackAvailable_Implementation();
}

void AMKAbilityCharacter::SetAttacking_Implementation(const bool Value)
{
	CombatComponent->SetAttacking_Implementation(Value);
}

void AMKAbilityCharacter::SetComboWindowOpen_Implementation(const bool Open)
{
	CombatComponent->SetComboWindowOpen_Implementation(Open);
}

void AMKAbilityCharacter::SetNextAttackAvailable_Implementation(const bool Available)
{
	CombatComponent->SetNextAttackAvailable_Implementation(Available);
}

void AMKAbilityCharacter::SetSectionName_Implementation(const FName& Name)
{
	CombatComponent->SetSectionName_Implementation(Name);
}

void AMKAbilityCharacter::TryComboAttack_Implementation()
{
	CombatComponent->TryComboAttack_Implementation();
}

void AMKAbilityCharacter::TrySectionAttack_Implementation(FName SectionName)
{
	CombatComponent->TrySectionAttack_Implementation(SectionName);
}

AActor* AMKAbilityCharacter::GetTargetActor_Implementation() const
{
	return CombatComponent->GetTargetActor_Implementation();
}

void AMKAbilityCharacter::PreAttack_Implementation(TSubclassOf<UGameplayEffect> Effect, float Level)
{
	CombatComponent->PreAttack_Implementation(Effect, Level);
}

void AMKAbilityCharacter::PostAttack_Implementation()
{
	CombatComponent->PostAttack_Implementation();
}

void AMKAbilityCharacter::PlayHitReact_Implementation(const FVector&               ImpactLocation, float Damage,
													  const FGameplayTagContainer& AdditionalTags)
{
	CombatComponent->PlayHitReact_Implementation(ImpactLocation, Damage, AdditionalTags);
}

void AMKAbilityCharacter::GetMeshParts(USkeletalMesh*& HelmetMesh, USkeletalMesh*& SuitMesh, USkeletalMesh*& ShoesMesh,
									   USkeletalMesh*& GlovesMesh, USkeletalMesh*& HeadMesh) const
{
	HelmetMesh = Helmet->GetSkeletalMeshAsset();
	SuitMesh   = Suit->GetSkeletalMeshAsset();
	ShoesMesh  = Shoes->GetSkeletalMeshAsset();
	GlovesMesh = Gloves->GetSkeletalMeshAsset();
	HeadMesh   = Head->GetSkeletalMeshAsset();
}

bool AMKAbilityCharacter::IsAlive() const
{
	float CurrentHealth = AbilitySystemComponent->GetNumericAttribute(UAttributeSet_Health::GetCurrentHealthAttribute());
	UE_LOG(LogTemp, Warning, TEXT("Current Health: %f"), CurrentHealth);
	return CurrentHealth > 1;
}


void AMKAbilityCharacter::UpdateMotionWarpingTargetLocation(FName WarpName, const FVector& TargetLocation)
{
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(WarpName, TargetLocation);
}

void AMKAbilityCharacter::UpdateMotionWarpingTargetLocationAndRotation(
	FName           WarpName, const FVector& TargetLocation,
	const FRotator& TargetRotation)
{
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(
																		 WarpName,
																		 TargetLocation,
																		 TargetRotation);
}

// void AMKAbilityCharacter::PlayHitReact_Implementation(EHitReactDirection Direction)
// {
// 	if (IsAlive())
// 	{
// 		OnCharacterBaseHitReact.Broadcast(Direction);
// 	}
// 	else
// 	{
// 		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//
// 		OnAbilityCharacterDie.Broadcast();
// 	}
//
// }
