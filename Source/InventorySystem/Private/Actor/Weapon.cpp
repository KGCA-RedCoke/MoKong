// CopyRight KGCA - Team RedCoke


#include "Actor/Weapon.h"

#include "AbilitySystemGlobals.h"
#include "DidItHitActorComponent.h"


AWeapon::AWeapon()
{
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot.Get());

	SM_Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticMesh"));
	SM_Weapon->SetupAttachment(GetRootComponent());

	SK_Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	SK_Weapon->SetupAttachment(GetRootComponent());
	SetActorEnableCollision(false);

	DidItHitActor = CreateDefaultSubobject<UDidItHitActorComponent>(TEXT("TraceComponent"));

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::InitializeData()
{
	if (ItemData.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemData is null"));
		return;
	}

	ItemSpecPtr = ItemData.GetRow<FMKEquipmentItemSpec>("Cannot find row data");
	ItemSpec    = *ItemSpecPtr;
	WeaponData  = *static_cast<FMKEquipmentItemSpec*>(ItemSpecPtr);
}

void AWeapon::InitializeWeapon(USkeletalMeshComponent* InOwnerMeshComponent)
{
	check(WeaponDataAsset);

	AvatarMeshComponent = InOwnerMeshComponent;

	SwapWeapon(ItemData.RowName);

	AttachWeapon();

	if (WeaponData.SK_Mesh)
	{
		DidItHitActor->SetupVariables(SK_Weapon.Get(), this);
		SK_Weapon->SetLeaderPoseComponent(InOwnerMeshComponent);
	}
	else
	{
		DidItHitActor->SetupVariables(SM_Weapon.Get(), this);
	}

	DidItHitActor->MyActorsToIgnore.AddUnique(AvatarMeshComponent->GetOwner());

	AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InOwnerMeshComponent->GetOwner());
}

void AWeapon::SwapWeapon(FName RowName)
{
	ItemData.RowName = RowName;

	InitializeData();

	if (!WeaponData.SM_Mesh.IsNull())
	{
		SM_Weapon->SetStaticMesh(WeaponData.SM_Mesh.LoadSynchronous());
	}
	if (!WeaponData.SK_Mesh.IsNull())
	{
		SK_Weapon->SetSkeletalMesh(WeaponData.SK_Mesh.LoadSynchronous());
	}

}

void AWeapon::AttachWeapon()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	GetRootComponent()->SetVisibility(true, true);

	// Socket Attach
	AttachToComponent(AvatarMeshComponent,
					  FAttachmentTransformRules::KeepRelativeTransform,
					  WeaponDataAsset->WeaponAttachmentData.HandSocket);
}

void AWeapon::DettachWeapon()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	GetRootComponent()->SetVisibility(false, true);
}

void AWeapon::PreAttack(TSubclassOf<UGameplayEffect> Effect, float Level)
{
	DamageGameplayEffect = Effect;
	EffectLevel          = Level;

	DidItHitActor->ToggleTraceCheck(true);
}

void AWeapon::PostAttack()
{
	DidItHitActor->ToggleTraceCheck(false);
}

const FWeaponCombatData& AWeapon::GetWeaponCombatData() const
{
	return WeaponDataAsset->WeaponCombatData;
}

const FWeaponMontageSet& AWeapon::GetWeaponMontageSet() const
{
	return WeaponDataAsset->WeaponMontageSet;
}
