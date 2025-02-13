// CopyRight KGCA - Team RedCoke


#include "Character/MokongEnemy.h"

#include "Components/WidgetComponent.h"
#include "UI/LockOnWidget.h"


// Sets default values
AMokongEnemy::AMokongEnemy()
{
	LockOnWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidgetComponent"));
	LockOnWidgetComponent->SetupAttachment(GetMesh());
	LockOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	LockOnWidgetComponent->SetDrawSize(FVector2D{16, 16});
	LockOnWidgetComponent->SetWidgetClass(ULockOnWidget::StaticClass());
	LockOnWidgetComponent->SetVisibility(false);
}

// Called when the game starts or when spawned
void AMokongEnemy::BeginPlay()
{
	Super::BeginPlay();

}

void AMokongEnemy::ShowLockOnWidget(bool bShow)
{
	auto* lockOnWidgetClassWidget = Cast<ULockOnWidget>(LockOnWidgetComponent->GetWidget());
	if (lockOnWidgetClassWidget)
	{
		LockOnWidgetComponent->SetVisibility(bShow);
		lockOnWidgetClassWidget->ShowWidget(bShow);
	}
}
