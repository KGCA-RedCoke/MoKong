// CopyRight KGCA - Team RedCoke


#include "CombatSystem/Components/CombatSystemComp.h"

#include "AbilitySystemComponent.h"
#include "Ability/MKAbilitySystemComponent.h"
#include "Actor/Weapon/WeaponBase.h"

UCombatSystem::UCombatSystem()
	: bReadyToNextAttack(true)	// 처음에는 공격 가능 상태로 시작
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatSystem::InitializeCombatSystem(USkeletalMeshComponent* InAvatarMeshComponent)
{
	AvatarMeshComponent = InAvatarMeshComponent;

	AbilitySystemComponent = UMKAbilitySystemComponent::GetAbilitySystemComponentFromActor(GetOwner());
	check(AbilitySystemComponent);

	// Weapon의 초기 설정 변수를 설정 후에 FinishSpawning을 호출해 블루프린트의 Construction(데이터 대입)을 실행
	if (WeaponClasses.IsEmpty())
		return;

	if (AWeaponBase* NewWeapon = GetWorld()->SpawnActorDeferred<AWeaponBase>(
																			 WeaponClasses[CurrentWeaponType],
																			 FTransform::Identity,
																			 GetOwner()))
	{
		NewWeapon->FinishSpawning(FTransform::Identity);
		NewWeapon->InitializeWeapon(InAvatarMeshComponent);
		CurrentWeapon = NewWeapon;
	}

	check(CurrentWeapon);
}

void UCombatSystem::TryComboAttack_Implementation()
{
	AbilitySystemComponent->TryActivateAbilityByName("PrimaryAttack");
}

void UCombatSystem::TrySectionAttack_Implementation()
{
	ICombatInterface::TrySectionAttack_Implementation();
}

void UCombatSystem::PreAttack_Implementation(int32 EffectLevel)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->EffectContext = AbilitySystemComponent->MakeEffectContext();

		CurrentWeapon->DamageEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CurrentWeapon->
				 DamageGameplayEffect,
				 EffectLevel,
				 CurrentWeapon->EffectContext);

		CurrentWeapon->PreAttack();
	}

}

void UCombatSystem::PostAttack_Implementation()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->PostAttack();
	}
}

UCombatSystem* UCombatSystem::GetCombatComponent_Implementation()
{
	return this;
}

FName UCombatSystem::GetSectionName_Implementation() const
{
	return CurrentSectionName;
}

bool UCombatSystem::IsNextAttackAvailable_Implementation() const
{
	return bReadyToNextAttack;
}

bool UCombatSystem::IsComboWindowOpen_Implementation() const
{
	return bComboWindowOpened;
}

void UCombatSystem::SetAttacking_Implementation(const bool Value)
{
	bAttacking = Value;
}

void UCombatSystem::SetNextAttackAvailable_Implementation(const bool Available)
{
	bReadyToNextAttack = Available;
}

void UCombatSystem::SetComboWindowOpen_Implementation(const bool Open)
{
	bComboWindowOpened = Open;
}

void UCombatSystem::SetSectionName_Implementation(const FName& Name)
{
	CurrentSectionName = Name;
}

AActor* UCombatSystem::GetTargetActor_Implementation() const
{
	return TargetActor.Get();
}


bool UCombatSystem::CheckCommit()
{
	if (bComboWindowOpened)
	{
		bReadyToNextAttack = true;
	}

	const FName PlayingSection = AvatarMeshComponent->GetAnimInstance()->Montage_GetCurrentSection();

	if (PlayingSection.IsEqual(CurrentSectionName))
	{
		return false;
	}

	return bReadyToNextAttack;
}

void UCombatSystem::SetTargetActor(AActor* InTargetActor) { TargetActor = InTargetActor; }

void UCombatSystem::ResetComboData()
{
	CurrentSectionName = "1";
	bComboWindowOpened = bAttacking = false;
	bReadyToNextAttack = true;

	PostAttack_Implementation();
}

void UCombatSystem::UpdateComboData(TObjectPtr<UAnimMontage>& MontageToPlay, FName& SectionToPlay, bool bInAir)
{
	const auto& MontageList = CurrentWeapon->GetWeaponCombatData();

	MontageToPlay = bInAir
						? MontageList.AirComboMontages["Default"].Get()
						: MontageList.GroundComboMontages["Default"].Get();

	if (MontageToPlay)
	{
		SectionToPlay      = CurrentSectionName;
		bReadyToNextAttack = false;
	}
}

void UCombatSystem::SwapWeapon(FName RowName)
{}
