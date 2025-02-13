// CopyRight KGCA - Team RedCoke


#include "Data/MKAbilitySystemData.h"

#include "GameplayAbilitySpecHandle.h"
#include "Ability/MKAbilitySystemComponent.h"
#include "Ability/MKGameplayAbility.h"

bool FMKGameplayEffectContainerSpec::HasValidEffects() const
{
	return TargetGameplayEffectSpecs.Num() > 0;
}

bool FMKGameplayEffectContainerSpec::HasValidTargets() const
{
	return TargetData.Num() > 0;
}

void FMKGameplayEffectContainerSpec::AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
{
	for (const FHitResult& HitResult : HitResults)
	{
		FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		TargetData.Add(NewData);
	}

	if (TargetActors.Num() > 0)
	{
		FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
		NewData->TargetActorArray.Append(TargetActors);
		TargetData.Add(NewData);
	}
}

void FMKAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void UMKAbilitySet::GiveToAbilitySystem(UMKAbilitySystemComponent* InASC, FMKAbilitySet_GrantedHandles* OutGrantedHandles,
										UObject*                   SourceObject) const
{
	check(InASC)

	if (!InASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	int32 ArrayIndex = 0;
	for (const FMKAbilitySet_GameplayAbility& AbilityToGrant : GrantedGameplayAbilities)
	{
		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogTemp,
				   Error,
				   TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."),
				   ArrayIndex,
				   *GetNameSafe(this));
			continue;
		}

		UMKGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UMKGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = InASC->GiveAbility(AbilitySpec);

		InASC->AddSpecHandle(AbilityCDO->GetAbilityID(), AbilitySpecHandle);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}

		ArrayIndex++;
	}
}
