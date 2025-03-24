// CopyRight KGCA - Team RedCoke


#include "Character/MokongEnemy.h"

#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "DidItHitActorComponent.h"
#include "ItemTypes.h"
#include "Ability/MKAbilitySystemComponent.h"
#include "AttributeSets/AttributeSet_Health.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/MKPlayer.h"
#include "Component/InventorySystemComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/EnemyHealthBar.h"
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

	EnemyHPWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHPWidgetComponent"));
	EnemyHPWidgetComponent->SetupAttachment(GetMesh(), TEXT("FX_head"));
	EnemyHPWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	EnemyHPWidgetComponent->SetDrawSize(FVector2D{100, 8});
	EnemyHPWidgetComponent->SetVisibility(false);

	PrimaryAttackTrace = CreateDefaultSubobject<UDidItHitActorComponent>(TEXT("PrimaryAttackTrace"));
	PrimaryAttackTrace->OnItemAdded.AddUniqueDynamic(this, &AMokongEnemy::OnHitPlayer);

	OnAbilityCharacterDie.AddUniqueDynamic(this, &AMokongEnemy::OnDeath);
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

void AMokongEnemy::OnPlayerJXSQ_Implementation(AActor* NewTarget)
{
	// Set Target new (replicated player)
	TargetActor = NewTarget;

	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBComp = AIC->GetBlackboardComponent())
		{
			BBComp->SetValueAsObject(FName("Target"), TargetActor);
		}
	}
}

void AMokongEnemy::OnPlayerJXSQExpired_Implementation()
{
	TargetActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ensure(TargetActor);

	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBComp = AIC->GetBlackboardComponent())
		{
			BBComp->SetValueAsObject(FName("Target"), TargetActor);
		}
	}
}

void AMokongEnemy::PreAttack_Implementation(TSubclassOf<class UGameplayEffect> Effect, float Level)
{
	Super::PreAttack_Implementation(Effect, Level);

	if (PrimaryAttackTrace)
	{
		PrimaryAttackTrace->ToggleTraceCheck(true);
	}

	DamageSpec = AbilitySystemComponent->MakeOutgoingSpec(Effect, Level, AbilitySystemComponent->MakeEffectContext());
}

void AMokongEnemy::PostAttack_Implementation()
{
	Super::PostAttack_Implementation();

	if (PrimaryAttackTrace)
	{
		PrimaryAttackTrace->ToggleTraceCheck(false);
	}
}

void AMokongEnemy::PlayHitReact_Implementation(const FVector&               ImpactLocation, float Damage,
											   const FGameplayTagContainer& AdditionalTags)
{
	Super::PlayHitReact_Implementation(ImpactLocation, Damage, AdditionalTags);

	if (!EnemyHPWidgetComponent->IsWidgetVisible())
	{
		ShowEnemyHPWidget(true);
	}

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

void AMokongEnemy::ShowEnemyHPWidget(bool bShow)
{
	auto* EnemyHPBar = Cast<UEnemyHealthBar>(EnemyHPWidgetComponent->GetWidget());
	if (EnemyHPBar)
	{
		EnemyHPWidgetComponent->SetVisibility(bShow);
	}
}

void AMokongEnemy::GenDeathItems_Implementation()
{
	AMKPlayer* PlayerRef = Cast<AMKPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// 경험치, 골드 부여
	// 임시방편으로 몬스터 체력에 따라서 
	if (auto* ASC = PlayerRef->GetMKAbilitySystemComponent())
	{
		if (RewardAttributes)
		{
			float MaxHealth = AbilitySystemComponent->
					GetNumericAttribute(UAttributeSet_Health::GetMaximumHealthAttribute());


			float Gold = FMath::RandRange(MaxHealth * 0.25f, MaxHealth * 0.35f);
			float Exp  = FMath::RandRange(MaxHealth * 0.3f, MaxHealth * 0.4f);

			FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
			FGameplayEffectSpecHandle    SpecHandle    = AbilitySystemComponent->
					MakeOutgoingSpec(RewardAttributes, 1, ContextHandle);

			SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Rewards.Gold")),
													 Gold);
			SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Rewards.EXP")),
													 Exp);

			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}


		for (const auto& Item : RewardItems)
		{
			if (FMKItemSpec* ItemSpec = Item.GetRow<FMKItemSpec>("Could not find row"))
			{
				PlayerRef->GetInventorySystemComponent()->AddToStackInInventory(
																				*ItemSpec,
																				0);
			}
		}
	}
}

void AMokongEnemy::OnDeath_Implementation()
{
	if (EnemyHPWidgetComponent->IsWidgetVisible())
	{
		ShowEnemyHPWidget(false);
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
