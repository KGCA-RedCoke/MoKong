// CopyRight KGCA - Team RedCoke


#include "Player/MKPlayerController.h"
#include "UI/PlayerHUDWidget.h"

void AMKPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerAttributeWidget = CreateWidget<UPlayerHUDWidget>(this, AttributeWidgetClass, FName("PlayerAttributeWidget"));
	if (PlayerAttributeWidget)
	{
		// Initialize가 성공할 때 까지 다음 Tick에서 다시 시도
		GetWorldTimerManager().SetTimer(NextTickTimer,
										[this](){
											if (PlayerAttributeWidget->
												InitializeAbilitySystemWidget(GetMKAbilitySystemComponent()))
											{
												PlayerAttributeWidget->AddToViewport();

												GetWorldTimerManager().ClearTimer(NextTickTimer);
											}
										},
										0.1f,
										true);
	}
}
