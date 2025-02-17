// CopyRight KGCA - Team RedCoke


#include "AI/CrowdEnemyAIController.h"

#include "Ability/MKAbilitySystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/MokongEnemy.h"
#include "Perception/AIPerceptionComponent.h"


// Sets default values
ACrowdEnemyAIController::ACrowdEnemyAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void ACrowdEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AMokongEnemy* Enemy = Cast<AMokongEnemy>(InPawn))
	{
		UBehaviorTree* BT = Enemy->GetBehaviorTree();

		OnAIDetectTarget.Clear();
		OnAIDetectSound.Clear();

		OnAIDetectTarget.AddUniqueDynamic(Enemy, &AMokongEnemy::OnPlayerSeen);
		OnAIDetectSound.AddUniqueDynamic(Enemy, &AMokongEnemy::OnHeardSomething);

		// 개별 BT 실행	
		RunBehaviorTree(BT);

		// Base 범위 설정
		float AttackRange, DefenseRange;
		Enemy->GetIdealRange_Implementation(AttackRange, DefenseRange);

		// Blackboard 설정
		Blackboard->SetValueAsEnum(Key_State, static_cast<uint8>(EAIEnemyState::Idle));
		Blackboard->SetValueAsFloat(Key_AttackRadius, AttackRange);
		Blackboard->SetValueAsFloat(Key_DefenseRadius, DefenseRange);

		Blackboard->SetValueAsObject(Key_ASC, Enemy->GetMKAbilitySystemComponent());
	}
}

void ACrowdEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}


void ACrowdEnemyAIController::BeginPlay()
{
	Super::BeginPlay();


}

void ACrowdEnemyAIController::HandleSenseSight(AActor* Target)
{
	OnAIDetectTarget.Broadcast(Target);
}

void ACrowdEnemyAIController::HandleSenseHearing(const FVector& Location)
{
	OnAIDetectSound.Broadcast(Location);
}
