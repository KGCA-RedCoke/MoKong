// CopyRight KGCA - Team RedCoke


#include "CombatSystem/Notify/AnimNotifyState_ComboWindow.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "MKAbilitySystemBlueprintLibrary.h"
#include "Ability/MKAbilitySystemComponent.h"
#include "Ability/MKGameplayAbility.h"
#include "CombatSystem/Interface/CombatInterface.h"

UAnimNotifyState_ComboWindow::UAnimNotifyState_ComboWindow()
{
	NotifyColor         = FColor(255, 164, 14, 255);
	bShouldFireInEditor = false;
}

void UAnimNotifyState_ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
											   float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp->GetOwner()->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()))
	{
		// 입력이 가능한 구간 설정
		ICombatInterface::Execute_SetComboWindowOpen(MeshComp->GetOwner(), true);
	}
}

void UAnimNotifyState_ComboWindow::NotifyEnd(USkeletalMeshComponent*          MeshComp, UAnimSequenceBase* Animation,
											 const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp->GetOwner()->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()))
	{
		// 입력이 가능한 구간 해제
		ICombatInterface::Execute_SetComboWindowOpen(MeshComp->GetOwner(), false);

		if (ICombatInterface::Execute_IsNextAttackAvailable(MeshComp->GetOwner()))
		{
			ICombatInterface::Execute_SetSectionName(MeshComp->GetOwner(), NextComboSection);

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), FGameplayTag::RequestGameplayTag("Event.Montage.NextSection"), FGameplayEventData());
			
			// UMKAbilitySystemComponent* AbilitySystemComponent =
			// 		UMKAbilitySystemComponent::GetAbilitySystemComponentFromActor(MeshComp->GetOwner());
			// if (AbilitySystemComponent)
			// {
			// 	AbilitySystemComponent->TryActivateAbilityByName("PrimaryAttack");
			// }
		}
		else
		{
			ICombatInterface::Execute_SetSectionName(MeshComp->GetOwner(), "1");
			ICombatInterface::Execute_SetNextAttackAvailable(MeshComp->GetOwner(), true);
		}
	}
}

FString UAnimNotifyState_ComboWindow::GetNotifyName_Implementation() const
{
	return UTF8TEXT("콤보 가능 구간");
}
