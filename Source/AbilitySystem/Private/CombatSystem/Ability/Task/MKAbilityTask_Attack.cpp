// CopyRight KGCA - Team RedCoke


#include "CombatSystem/Ability/Task/MKAbilityTask_Attack.h"

#include "AbilitySystemComponent.h"
#include "CombatSystem/Components/CombatSystemComp.h"

UMKAbilityTask_Attack* UMKAbilityTask_Attack::CreateAttackTask(UGameplayAbility* OwningAbility)
{
	UMKAbilityTask_Attack* MyTask = NewAbilityTask<UMKAbilityTask_Attack>(OwningAbility);
	return MyTask;
}

void UMKAbilityTask_Attack::Activate()
{
	HandleCombatState();
}

void UMKAbilityTask_Attack::HandleCombatState()
{

	if (GetAvatarActor()->Implements<UCombatInterface>())
	{

		if (ICombatInterface::Execute_IsComboWindowOpen(GetAvatarActor()))
		{
			ICombatInterface::Execute_SetNextAttackAvailable(GetAvatarActor(), true);
			OnCanSaveAttack.Broadcast();
			return;
		}

		OnCanAttack.Broadcast();
	}
}
