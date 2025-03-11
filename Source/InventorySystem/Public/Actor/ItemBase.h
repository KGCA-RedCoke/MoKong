// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UCLASS()
class INVENTORYSYSTEM_API AItemBase : public AActor
{
	GENERATED_BODY()

public:
	AItemBase();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void InitializeData();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Item")
	FDataTableRowHandle ItemData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Item")
	FMKItemSpec ItemSpec;

	FMKItemSpec* ItemSpecPtr;
};
