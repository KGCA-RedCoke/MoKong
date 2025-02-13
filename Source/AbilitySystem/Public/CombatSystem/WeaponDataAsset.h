// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "AttackSystemTypes.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType = EWeaponType::Unarmed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponAttachmentData WeaponAttachmentData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponMontageSet WeaponMontageSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponCombatData WeaponCombatData;
};
