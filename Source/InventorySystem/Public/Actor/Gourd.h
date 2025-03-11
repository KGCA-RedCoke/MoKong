// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Gourd.generated.h"

class UAbilitySystemComponent;

UCLASS()
class INVENTORYSYSTEM_API AGourd : public AItemBase
{
	GENERATED_BODY()

public:
	AGourd();

	virtual void BeginPlay() override;

	virtual void InitializeData() override;

	UFUNCTION(BlueprintCallable, Category="Item|Gourd")
	bool CanConsume() const;

	UFUNCTION(BlueprintCallable, Category="Item|Gourd")
	void SwitchData(FName RowName);

	UFUNCTION(BlueprintCallable, Category="Item|Gourd")
	void ConsumeGourd();

public:
	UPROPERTY(BlueprintReadOnly, Category="Item")
	FMKGourdItemSpec GourdData;

	UPROPERTY(BlueprintReadOnly, Category="Item")
	float GourdLevel;	// 호리병 업그레이드 레벨

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> SK_Mesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
