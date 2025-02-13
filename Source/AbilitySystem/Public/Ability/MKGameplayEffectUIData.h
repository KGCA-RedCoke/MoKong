// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectUIData.h"
#include "MKGameplayEffectUIData.generated.h"

class UEffectWidgetController;
/**
 * 
 */
UCLASS(DisplayName="Custom UI Data", Config = Game)
class ABILITYSYSTEM_API UMKGameplayEffectUIData : public UGameplayEffectUIData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data, meta = (MultiLine = "true"))
	FText Title;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data, meta = (MultiLine = "true"))
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	TSoftObjectPtr<UTexture2D> IconTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	TSoftObjectPtr<UMaterialInterface> IconMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Data)
	TSubclassOf<UEffectWidgetController> EffectWidgetController;

	UFUNCTION(BlueprintPure, Category = "Custom Ability System")
	static const UMKGameplayEffectUIData* GetGameplayEffectUIData(FActiveGameplayEffectHandle ActiveEffectHandle);

	UFUNCTION(BlueprintPure, Category = "Custom Ability System")
	static const UMKGameplayEffectUIData* GetGameplayEffectUIDataFromActiveEffect(
		const FActiveGameplayEffect& ActiveEffect);
};
