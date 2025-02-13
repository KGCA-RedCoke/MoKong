// CopyRight KGCA - Team RedCoke


#include "Data/MKTargetType.h"
#include "GameFramework/Character.h"
#include "Abilities/GameplayAbilityTypes.h"

void UMKTargetType::GetTargets_Implementation(ACharacter*        TargetingCharacter, AActor*    TargetingActor,
											  FGameplayEventData EventData, TArray<FHitResult>& OutHitResults,
											  TArray<AActor*>&   OutActor) const {}

void UTargetType_UseOwner::GetTargets_Implementation(ACharacter*        TargetingCharacter, AActor*    TargetingActor,
													 FGameplayEventData EventData, TArray<FHitResult>& OutHitResults,
													 TArray<AActor*>&   OutActor) const
{
	OutActor.Add(TargetingCharacter);
}

void UTargetType_UseEventData::GetTargets_Implementation(ACharacter*        TargetingCharacter, AActor*    TargetingActor,
														 FGameplayEventData EventData, TArray<FHitResult>& OutHitResults,
														 TArray<AActor*>&   OutActor) const
{
	if (const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult())
	{
		OutHitResults.Add(*FoundHitResult);
	}
	else if (EventData.TargetData.Num() > 0)
	{
		OutHitResults.Add(*EventData.TargetData.Get(0)->GetHitResult());
	}

	else if (EventData.Target)
	{
		OutActor.Add(const_cast<AActor*>(EventData.Target.Get()));
	}
}
