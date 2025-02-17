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

float AMokongEnemy::SetMovementSpeed_Implementation(EAIMovementSpeed NewSpeed)
{
	return IEnemyAIInterface::SetMovementSpeed_Implementation(NewSpeed);
}

void AMokongEnemy::GetIdealRange_Implementation(float& OutAttackRange, float& OutDetectRange)
{
	IEnemyAIInterface::GetIdealRange_Implementation(OutAttackRange, OutDetectRange);
}

void AMokongEnemy::OnPlayerSeen_Implementation(AActor* Target)
{
	IEnemyAIInterface::OnPlayerSeen_Implementation(Target);
}

void AMokongEnemy::OnHeardSomething_Implementation(const FVector& Location)
{
	IEnemyAIInterface::OnHeardSomething_Implementation(Location);
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
