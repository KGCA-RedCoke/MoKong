// CopyRight KGCA - Team RedCoke


#include "CombatSystem/Notify/AnimNotifyState_Action.h"

#include "CombatSystem/Components/CombatSystemComp.h"

void UAnimNotifyState_Action::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
										  float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (CheckComponentIsValid(MeshComp))
	{
		// CombatSystem->SetActionState(ActionState);
	}
}

void UAnimNotifyState_Action::NotifyEnd(USkeletalMeshComponent*          MeshComp, UAnimSequenceBase* Animation,
										const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (CheckComponentIsValid(MeshComp))
	{
		// if (CombatSystem->GetActionState() == ActionState)
		// {
		// 	CombatSystem->SetActionState(NextActionState);
		// }
	}
}

FString UAnimNotifyState_Action::GetNotifyName_Implementation() const
{
	switch (ActionState)
	{
	case ECombatState::Idle:
		return "Idle";
	case ECombatState::AttackWindowOpened:
		return "Window Opened";
	case ECombatState::Attacking:
		return "Attacking";
	case ECombatState::AttackFinished:
		return "Attack Finished";
	case ECombatState::WeaponHandling:
		return "Weapon Handling";
	case ECombatState::Etc:
		return "None";
	}
	return "Action State";
}

bool UAnimNotifyState_Action::CheckComponentIsValid(USkeletalMeshComponent* MeshComp)
{
	if (MeshComp->GetOwner() == nullptr)
	{
		return false;
	}

	if (CachedOwner != MeshComp->GetOwner())
	{
		CachedOwner  = MeshComp->GetOwner();
		CombatSystem = MeshComp->GetOwner()->FindComponentByClass<UCombatSystem>();
	}

	return CombatSystem.IsValid();
}
