// CopyRight KGCA - Team RedCoke


#include "Character/MokongEnemy.h"

#include "AbilitySystemGlobals.h"
#include "DidItHitActorComponent.h"
#include "Ability/MKAbilitySystemComponent.h"
#include "Character/MKPlayer.h"
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

	PrimaryAttackTrace = CreateDefaultSubobject<UDidItHitActorComponent>(TEXT("PrimaryAttackTrace"));
	PrimaryAttackTrace->OnItemAdded.AddDynamic(this, &AMokongEnemy::OnHitPlayer);
}

// Called when the game starts or when spawned
void AMokongEnemy::BeginPlay()
{
	Super::BeginPlay();

	// PrimaryAttackTrace->SetupVariables(GetMesh(), this);

}

float AMokongEnemy::SetMovementSpeed_Implementation(EAIMovementSpeed NewSpeed)
{
	return IEnemyAIInterface::SetMovementSpeed_Implementation(NewSpeed);
}

void AMokongEnemy::GetIdealRange_Implementation(float& OutAttackRange, float& OutDetectRange)
{
	Execute_GetIdealRange(this, OutAttackRange, OutDetectRange);
}

void AMokongEnemy::OnPlayerSeen_Implementation(AActor* Target)
{
	IEnemyAIInterface::OnPlayerSeen_Implementation(Target);
}

void AMokongEnemy::OnHeardSomething_Implementation(const FVector& Location)
{
	IEnemyAIInterface::OnHeardSomething_Implementation(Location);
}

void AMokongEnemy::PreAttack_Implementation(TSubclassOf<class UGameplayEffect> Effect, float Level)
{
	Super::PreAttack_Implementation(Effect, Level);

	PrimaryAttackTrace->ToggleTraceCheck(true);

	DamageSpec = AbilitySystemComponent->MakeOutgoingSpec(Effect, Level, AbilitySystemComponent->MakeEffectContext());
}

void AMokongEnemy::PostAttack_Implementation()
{
	Super::PostAttack_Implementation();

	PrimaryAttackTrace->ToggleTraceCheck(false);
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

void AMokongEnemy::OnHitPlayer_Implementation(FHitResult LastItem)
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(LastItem.GetActor()))
	{
		DamageSpec.Data->GetContext().AddHitResult(LastItem);

		ASC->ApplyGameplayEffectSpecToSelf(*DamageSpec.Data.Get());
	}
}
