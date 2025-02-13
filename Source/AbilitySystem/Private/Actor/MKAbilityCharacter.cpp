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

	DeathVfx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("VFX_Death"));

	AvatarMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AvatarMesh"));
	AvatarMesh->SetupAttachment(GetMesh());
	{
		Head      = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
		Helmet    = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Helmet"));
		UpperBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("UpperBody"));
		LowerBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LowerBody"));
		Shoes     = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoes"));
		Gloves    = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gloves"));

		Head->SetupAttachment(GetAvatarMesh());
		Helmet->SetupAttachment(GetAvatarMesh());
		UpperBody->SetupAttachment(GetAvatarMesh());
		LowerBody->SetupAttachment(GetAvatarMesh());
		Shoes->SetupAttachment(GetAvatarMesh());
		Gloves->SetupAttachment(GetAvatarMesh());

		Head->SetLeaderPoseComponent(GetAvatarMesh());
		Helmet->SetLeaderPoseComponent(GetAvatarMesh());
		UpperBody->SetLeaderPoseComponent(GetAvatarMesh());
		LowerBody->SetLeaderPoseComponent(GetAvatarMesh());
		Shoes->SetLeaderPoseComponent(GetAvatarMesh());
		Gloves->SetLeaderPoseComponent(GetAvatarMesh());
	}
}

void AMKAbilityCharacter::BeginPlay()
{
	Super::BeginPlay();

	CombatComponent->InitializeCombatSystem(GetMesh());
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

bool AMKAbilityCharacter::IsAttacking_Implementation() const
{
	return CombatComponent->IsAttacking_Implementation();
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

void AMKAbilityCharacter::TrySectionAttack_Implementation()
{
	CombatComponent->TrySectionAttack_Implementation();
}

AWeaponBase* AMKAbilityCharacter::GetCurrentWeapon_Implementation() const
{
	return CombatComponent->GetCurrentWeapon_Implementation();
}

AActor* AMKAbilityCharacter::GetTargetActor_Implementation() const
{
	return CombatComponent->GetTargetActor_Implementation();
}

void AMKAbilityCharacter::PreAttack_Implementation(int32 EffectLevel)
{
	CombatComponent->PreAttack_Implementation(EffectLevel);
}

void AMKAbilityCharacter::PostAttack_Implementation()
{
	CombatComponent->PostAttack_Implementation();
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

void AMKAbilityCharacter::PlayHitReact_Implementation(EHitReactDirection Direction)
{
	if (IsAlive())
	{
		OnCharacterBaseHitReact.Broadcast(Direction);
	}
	else
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		OnAbilityCharacterDie.Broadcast();
	}

}
