// CopyRight KGCA - Team RedCoke


#include "Calculations/DamageGameplayEffectExecutionCalculation.h"

#include "MKAbilitySystemBlueprintLibrary.h"
#include "MKGameplayTags.h"
#include "Ability/MKAbilitySystemComponent.h"
#include "AttributeSets/AttributeSet_Mokong.h"

const FMKDamageStatics* UDamageGameplayEffectExecutionCalculation::DamageStatics = nullptr;


UDamageGameplayEffectExecutionCalculation::UDamageGameplayEffectExecutionCalculation()
{
	if (!DamageStatics)
	{
		DamageStatics = &FMKDamageStatics::DamageStatics();
	}

	RelevantAttributesToCapture.Add(DamageStatics->ResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics->CurrentHealthDef);
	RelevantAttributesToCapture.Add(DamageStatics->MaximumHealthDef);
	RelevantAttributesToCapture.Add(DamageStatics->ReceivedBleedDef);
	RelevantAttributesToCapture.Add(DamageStatics->BleedingDef);
	RelevantAttributesToCapture.Add(DamageStatics->DamageDef);

	RelevantAttributesToCapture.Add(DamageStatics->AttackDef);
	RelevantAttributesToCapture.Add(DamageStatics->CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics->CriticalHitDamageDef);
}

void UDamageGameplayEffectExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput&           OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	if (!DamageStatics)
	{
		DamageStatics = &FMKDamageStatics::DamageStatics();
	}

	if (!DamageStatics)
	{
		return;
	}

	UMKAbilitySystemComponent* const TargetAbilitySystemComponent = Cast<
		UMKAbilitySystemComponent>(ExecutionParams.GetTargetAbilitySystemComponent());

	if (!TargetAbilitySystemComponent)
	{
		return;
	}

	// Feel free to uncomment any of those lines to have access to other objects.
	// UCustomAbilitySystemComponent* const SourceAbilitySystemComponent = Cast<UCustomAbilitySystemComponent>(ExecutionParams.GetSourceAbilitySystemComponent());

	// AActor* const SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	// AActor* const TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	// ACharacterBase* const SourceCharacter = Cast<ACharacterBase>(SourceActor);
	// ACharacterBase* const TargetCharacter = Cast<ACharacterBase>(TargetActor);

	const FGameplayEffectSpec&    Spec = ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters EvaluationParameters{};
	EvaluationParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// Apply Status Damage
	CalculateDamageAndApply(
							*TargetAbilitySystemComponent,
							Spec,
							1.f, // Decrease Current Health
							OutExecutionOutput,
							ExecutionParams,
							EvaluationParameters,
							DamageStatics->DamageDef,
							DamageStatics->DamageProperty /*DamageStatics->CurrentHealthProperty*/,
							MoKong::BasicAttack::TAG_DamageType_DirectDamage.GetTag(),
							MoKong::BasicAttack::TAG_Resistance_Modifier_DirectDamage.GetTag(),
							MoKong::BasicAttack::TAG_Resistance_Ignore_DirectDamage.GetTag(),
							MoKong::Statuses::TAG_Immunity_Bleeding.GetTag()
						   );


	CalculateDamageAndApply(
							*TargetAbilitySystemComponent,
							Spec,
							1.f, // Increase Bleed
							OutExecutionOutput,
							ExecutionParams,
							EvaluationParameters,
							DamageStatics->ReceivedBleedDef,
							DamageStatics->BleedingProperty,
							MoKong::Statuses::TAG_DamageType_Bleeding.GetTag(),
							MoKong::Statuses::TAG_Resistance_Modifier_Bleeding.GetTag(),
							MoKong::Statuses::TAG_Resistance_Ignore_Bleeding.GetTag(),
							MoKong::Statuses::TAG_Immunity_Bleeding.GetTag()
						   );
}

void UDamageGameplayEffectExecutionCalculation::CalculateDamageAndApply(const UMKAbilitySystemComponent& TargetASC,
																		const FGameplayEffectSpec&       Spec,
																		float                            DamageSign,
																		FGameplayEffectCustomExecutionOutput&
																		OutExecutionOutput,
																		const FGameplayEffectCustomExecutionParameters&
																		ExecutionParams,
																		const FAggregatorEvaluateParameters&
																		EvaluationParameters,
																		const FGameplayEffectAttributeCaptureDefinition&
																		AttributeDef, FProperty* OutputProperty,
																		const FGameplayTag&      DamageType,
																		const FGameplayTag&      ResistnceTag,
																		const FGameplayTag&      IgnoreResistanceTag,
																		const FGameplayTag&      ImmunityTag)
{
	// 어트리뷰트 값 가져오기
	float AttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
															   DamageStatics->AttackDef,
															   EvaluationParameters,
															   AttackPower);

	float CriticalChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
															   DamageStatics->CriticalHitChanceDef,
															   EvaluationParameters,
															   CriticalChance);

	float CriticalRate = 1.f; // 기본 배율 1.0 (치명타 아닐 경우)
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
															   DamageStatics->CriticalHitDamageDef,
															   EvaluationParameters,
															   CriticalRate);

	float DamageRaw = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttributeDef, EvaluationParameters, DamageRaw);
	DamageRaw = FMath::Max(DamageRaw, 0.f);

	if (DamageRaw == 0.f)
	{
		return;
	}

	// 치명타 확률 적용 (확률 기반)
	if (FMath::FRand() <= CriticalChance)
	{
		DamageRaw *= CriticalRate;
	}

	// Immune to this type of damage, return early
	if (ImmunityTag.IsValid() && TargetASC.HasMatchingGameplayTag(ImmunityTag))
	{
		return;
	}

	// Calculate the resistance attribute.

	/* We are using LoL Resistance logic
	 * https://www.quora.com/How-does-Resistance-work-in-League-of-Legends
	 * Resistance provides diminishing returns; as a champion’s Resistance increases, the percentage of damage reduced from physical attacks becomes less effective.
	 * This means that stacking Resistance is beneficial, but there are limits to its effectiveness.
	 * Note:	Removing Resistance will scale very fast (-50 Resistance = x 2 damage)
	 *			Could be clamped in the HealthAttributeSet to never go below 0.
	 *
	 * We are using KaosSpectrum implementation of https://www.thegames.dev/?p=165 where:
	 * - Only one Resistance attribute is used for ALL Armor / resistance.
	 * - We implemented: Direct (raw) damage, Bleed damage (DoT) and Fire Damage (Elemental, almost full BP, except for the execution)
	 *
	 * The version you see below is _almost_ exactly the same as Kaos, except:
	 * - The Damage Type is not specified in the GE Asset tag
	 * - Added based on the type of damage that is handled here (see DamageTypeTag)
	 *
	 * This means that ONE call to this Execution can handle MULTIPLE damage at the same time and MULTIPLE RESISTANCE calcuations
	 */

	float Resistance_DamageMultiplier = 1.f;

	const bool bIgnoreResistance =
			IgnoreResistanceTag.IsValid()	// 저항 태그 존재 
			&& EvaluationParameters.SourceTags->HasAny(IgnoreResistanceTag.GetSingleTagContainer());
	if (!bIgnoreResistance)
	{
		// The GE doesn't have any tag bypassing the resistance, so we calculate.
		FAggregatorEvaluateParameters EvaluationParameterWithDamageType{};
		bool                          bUseDynamicDamageTypeTag = false;

		// We'll calculate the current resistance, and the multiplier.
		if (DamageType.IsValid())
		{
			EvaluationParameterWithDamageType = EvaluationParameters;

			// Add the damage tag of this type of damage. 
			FGameplayTagContainer SourceEvaluationWithDamageType = *EvaluationParameters.SourceTags;
			SourceEvaluationWithDamageType.AddTag(DamageType);
			EvaluationParameterWithDamageType.SourceTags = &SourceEvaluationWithDamageType;

			bUseDynamicDamageTypeTag = true;
		}

		// This number will be added to the base resistance. 0 means no damage reduction/increase.
		float CurrentResistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
																   DamageStatics->ResistanceDef,
																   bUseDynamicDamageTypeTag
																	   ? EvaluationParameterWithDamageType
																	   : EvaluationParameters,
																   CurrentResistance
																  );
		CurrentResistance = FMath::Clamp(CurrentResistance,
										 UAttributeSet_Resistance::RESISTANCE_MIN,
										 UAttributeSet_Resistance::RESISTANCE_MAX);

		Resistance_DamageMultiplier =
				UMKAbilitySystemBlueprintLibrary::CalculateEffectiveResistance(CurrentResistance);
	}

	// Calculate the final damage
	const float FinalDamage = DamageRaw * Resistance_DamageMultiplier;

	// It's too close to 0 to consider anything, return early.
	if (FinalDamage <= 0.f || FMath::IsNearlyZero(DamageRaw, 0.01f))
	{
		return;
	}

	OutExecutionOutput.AddOutputModifier(
										 FGameplayModifierEvaluatedData(
																		OutputProperty,
																		EGameplayModOp::Additive,
																		FinalDamage * FMath::Sign(DamageSign)
																	   )
										);
}
