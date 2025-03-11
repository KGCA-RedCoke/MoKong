// CopyRight KGCA - Team RedCoke


#include "Actor/ItemBase.h"


// Sets default values
AItemBase::AItemBase()
{}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeData();
}

void AItemBase::InitializeData()
{
	if (ItemData.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemData is null"));
		return;
	}

	ItemSpecPtr = ItemData.GetRow<FMKItemSpec>("Cannot find row data");
	ItemSpec    = *ItemSpecPtr;

}
