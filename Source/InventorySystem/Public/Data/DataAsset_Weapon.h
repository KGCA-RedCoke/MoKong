// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.h"
#include "Engine/DataAsset.h"
#include "DataAsset_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UDataAsset_Weapon : public UDataAsset
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
