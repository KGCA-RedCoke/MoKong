// CopyRight KGCA - Team RedCoke


#include "AI/BossEnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/MokongEnemy.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABossEnemyAIController::ABossEnemyAIController()
{}

void ABossEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AMokongEnemy* Enemy = Cast<AMokongEnemy>(InPawn))
	{
		UBehaviorTree* BT = Enemy->GetBehaviorTree();

		// 개별 BT 실행	
		RunBehaviorTree(BT);

		// Base 범위 설정
		float AttackRange, DefenseRange;
		Enemy->GetIdealRange_Implementation(AttackRange, DefenseRange);

		// Blackboard 설정
		ACharacter* PlayerRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		Blackboard->SetValueAsObject(Key_Target, PlayerRef);

		Blackboard->SetValueAsEnum(Key_State, static_cast<uint8>(EAIEnemyState::Idle));

		Blackboard->SetValueAsFloat(Key_AttackRadius, AttackRange);
		Blackboard->SetValueAsFloat(Key_DefenseRadius, DefenseRange);
	}
}

void ABossEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

// Called when the game starts or when spawned
void ABossEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABossEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
