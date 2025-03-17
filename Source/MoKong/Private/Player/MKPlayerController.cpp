// CopyRight KGCA - Team RedCoke


#include "Player/MKPlayerController.h"

#include "Character/MokongEnemy.h"
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

void AMKPlayerController::AddCombatEnemy(AMokongEnemy* Enemy)
{
	if (Enemy)
	{
		Enemy->ShowEnemyHPWidget(true);

		CurrentCombatEnemies.AddUnique(Enemy);
	}
}

void AMKPlayerController::RemoveCombatEnemy(AMokongEnemy* Enemy)
{
	if (Enemy)
	{
		Enemy->ShowLockOnWidget(false);
		Enemy->ShowEnemyHPWidget(false);

		CurrentCombatEnemies.Remove(Enemy);
	}
}

AMokongEnemy* AMKPlayerController::GetEnemy(AMokongEnemy* Enemy)
{
	if (CurrentCombatEnemies.Contains(Enemy))
	{
		return Enemy;
	}
	return nullptr;
}

void AMKPlayerController::BeginPlay()
{
	Super::BeginPlay();
}
