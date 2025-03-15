// CopyRight KGCA - Team RedCoke


#include "Player/WK_Staff.h"
#include "DidItHitActorComponent.h"
#include "Ability/MKAbilitySystemComponent.h"


AWK_Staff::AWK_Staff()
{
	DidItHitActor->OnItemAdded.AddUniqueDynamic(this, &ThisClass::OnHitActorAdded);
}

void AWK_Staff::BeginPlay()
{
	Super::BeginPlay();

}

void AWK_Staff::OnHitActorAdded(FHitResult LastItem)
{
	if (auto* TargetAbility = UMKAbilitySystemComponent::GetAbilitySystemComponentFromActor(LastItem.GetActor()))
	{
		EffectContext = AbilitySystemComponent->MakeEffectContext();

		DamageEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
																		  DamageGameplayEffect,
																		  EffectLevel,
																		  EffectContext);

		EffectContext.AddHitResult(LastItem);
		TargetAbility->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());

		// 공격 성공시 랜덤 편차(데미지에 따른) 충전량 추가
		if (ChargeFocusEffect)
		{
			EffectContext                               = AbilitySystemComponent->MakeEffectContext();
			FGameplayEffectSpecHandle FocusEffectHandle = AbilitySystemComponent->MakeOutgoingSpec(
				 ChargeFocusEffect,
				 EffectLevel,
				 EffectContext);

			float RandomFocusMultiplier = FMath::RandRange(0.8f, 1.2f);

			FocusEffectHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")),
															RandomFocusMultiplier);

			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*FocusEffectHandle.Data.Get());
		}
	}
}
