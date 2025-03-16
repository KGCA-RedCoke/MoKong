// CopyRight KGCA - Team RedCoke


#include "Player/MKPlayerController.h"

#include "Component/InventorySystemComponent.h"
#include "UI/PlayerHUDWidget.h"

AMKPlayerController::AMKPlayerController(const FObjectInitializer& ObjectInitializer)
{
	PlayerInventory = CreateDefaultSubobject<UInventorySystemComponent>(TEXT("PlayerInventory"));
}

void AMKPlayerController::ResetState_Implementation()
{
	PlayerCameraManager->StartCameraFade(0, 1.f, 5.f, FLinearColor::Black, false, true);
	PlayerAttributeWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AMKPlayerController::BeginPlay()
{
	Super::BeginPlay();
}
