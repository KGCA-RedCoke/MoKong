// CopyRight KGCA - Team RedCoke


#include "Actor/Weapon/WeaponBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "DidItHitActorComponent.h"
#include "MKAbilitySystemBlueprintLibrary.h"
#include "CombatSystem/WeaponDataAsset.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot.Get());

	SM_Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticMesh"));
	SM_Weapon->SetupAttachment(GetRootComponent());

	SK_Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	SK_Weapon->SetupAttachment(GetRootComponent());
	SetActorEnableCollision(false);

	DidItHitActor = CreateDefaultSubobject<UDidItHitActorComponent>(TEXT("DidItHit"));
}

void AWeaponBase::OnHitActorAdded(FHitResult LastItem)
{
	EffectContext.AddHitResult(LastItem);

	UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(LastItem.GetActor())->
			ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
}


void AWeaponBase::InitializeWeapon(USkeletalMeshComponent* InOwnerMeshComponent)
{
	check(WeaponDataAsset);

	AvatarMeshComponent = InOwnerMeshComponent;

	SwapWeapon(AttributeDataHandle.RowName);


	// Socket Attach
	SM_Weapon->AttachToComponent(AvatarMeshComponent,
								 FAttachmentTransformRules::KeepRelativeTransform,
								 WeaponDataAsset->WeaponAttachmentData.HolsterSocket);
	SK_Weapon->AttachToComponent(AvatarMeshComponent,
								 FAttachmentTransformRules::KeepRelativeTransform,
								 WeaponDataAsset->WeaponAttachmentData.HolsterSocket);

	DidItHitActor->SetupVariables(SK_Weapon.Get(), this);
	SK_Weapon->SetLeaderPoseComponent(InOwnerMeshComponent);

	DidItHitActor->MyActorsToIgnore.AddUnique(AvatarMeshComponent->GetOwner());
	DidItHitActor->OnItemAdded.AddUniqueDynamic(this, &AWeaponBase::OnHitActorAdded);
}

void AWeaponBase::PreAttack()
{
	DidItHitActor->ToggleTraceCheck(true);
}

void AWeaponBase::PostAttack()
{
	DidItHitActor->ToggleTraceCheck(false);
}

void AWeaponBase::SheathWeapon(int32 Index)
{
	if (AvatarMeshComponent)
	{
		if (UAnimInstance* AnimInstance = AvatarMeshComponent->GetAnimInstance())
		{
			if (auto* Montage = WeaponDataAsset->WeaponMontageSet.EquipMontage[Index].Get())
			{
				AnimInstance->Montage_Play(Montage);

				AnimInstance->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &AWeaponBase::OnHandGoesToWeapon);
			}
		}
	}
}

void AWeaponBase::UnSheathWeapon(int32 Index)
{
	if (AvatarMeshComponent)
	{
		if (UAnimInstance* AnimInstance = AvatarMeshComponent->GetAnimInstance())
		{
			if (auto* Montage = WeaponDataAsset->WeaponMontageSet.UnEquipMontage[Index].Get())
			{
				AnimInstance->Montage_Play(Montage);

				// AnimInstance->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &AWeaponBase::OnHandGoesToWeapon);
			}
		}
	}
}


void AWeaponBase::SwapWeapon(FName RowName)
{
	AttributeDataHandle.RowName = RowName;

	if (auto* FoundData = AttributeDataHandle.GetRow<FWeaponAttributeData>("Cannot find row data"))
	{
		AttributeData = *FoundData;

		if (AttributeData.WeaponStaticMesh)
		{
			SM_Weapon->SetStaticMesh(AttributeData.WeaponStaticMesh.Get());
		}
		if (AttributeData.WeaponSkeletalMesh)
		{
			SK_Weapon->SetSkeletalMesh(AttributeData.WeaponSkeletalMesh.Get());
		}

		// 소유자 Mesh에 따로 붙여야 함
		if (AttributeData.ScabbardStaticMesh)
		{}
		if (AttributeData.ScabbardSkeletalMesh)
		{}


	}
}

const FWeaponCombatData& AWeaponBase::GetWeaponCombatData() const
{
	return WeaponDataAsset->WeaponCombatData;
}

const FWeaponMontageSet& AWeaponBase::GetWeaponMontageSet() const
{
	return WeaponDataAsset->WeaponMontageSet;
}

void AWeaponBase::OnHandGoesToWeapon(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName.IsEqual("Sheath"))
	{
		// Socket Attach
		SM_Weapon->AttachToComponent(AvatarMeshComponent,
									 FAttachmentTransformRules::KeepRelativeTransform,
									 WeaponDataAsset->WeaponAttachmentData.HandSocket);
		SK_Weapon->AttachToComponent(AvatarMeshComponent,
									 FAttachmentTransformRules::KeepRelativeTransform,
									 WeaponDataAsset->WeaponAttachmentData.HandSocket);
		if (AvatarMeshComponent)
		{
			if (UAnimInstance* AnimInstance = AvatarMeshComponent->GetAnimInstance())
			{
				AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &AWeaponBase::OnHandGoesToWeapon);
			}
		}
	}
}
