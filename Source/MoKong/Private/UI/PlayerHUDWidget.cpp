// CopyRight KGCA - Team RedCoke


#include "UI/PlayerHUDWidget.h"
#include "Ability/MKAbilitySystemComponent.h"
#include "Ability/MKGameplayEffectUIData.h"
#include "AttributeSets/AttributeSet_Health.h"
#include "AttributeSets/AttributeSet_Mana.h"
#include "AttributeSets/AttributeSet_Mokong.h"
#include "AttributeSets/AttributeSet_Resistance.h"
#include "AttributeSets/AttributeSet_Stamina.h"

bool UPlayerHUDWidget::InitializeAbilitySystemWidget(UAbilitySystemComponent* InOwnerAbilitySystemComponent)
{
	UAbilitySystemComponent* OldAbilitySystemComponent = AbilitySystemComponent.Get();

	AbilitySystemComponent = Cast<UMKAbilitySystemComponent>(InOwnerAbilitySystemComponent);

	// The Ability System Component is invalid. Stop here and return false. 
	if (!GetOwnerAbilitySystemComponent())
	{
		return false;
	}

	// Reset any old Attribute Change Delegates if they are still bound.
	if (IsValid(OldAbilitySystemComponent))
	{
		ResetDelegateHandle(MaximumHealthChangeDelegate,
							OldAbilitySystemComponent,
							UAttributeSet_Health::GetMaximumHealthAttribute());
		ResetDelegateHandle(CurrentHealthChangeDelegate,
							OldAbilitySystemComponent,
							UAttributeSet_Health::GetCurrentHealthAttribute());
		ResetDelegateHandle(HealthRegenerationChangeDelegate,
							OldAbilitySystemComponent,
							UAttributeSet_Health::GetHealthRegenerationAttribute());
		ResetDelegateHandle(MaximumStaminaChangeDelegate,
							OldAbilitySystemComponent,
							UAttributeSet_Stamina::GetMaximumStaminaAttribute());
		ResetDelegateHandle(CurrentStaminaChangeDelegate,
							OldAbilitySystemComponent,
							UAttributeSet_Stamina::GetCurrentStaminaAttribute());
		ResetDelegateHandle(StaminaRegenerationChangeDelegate,
							OldAbilitySystemComponent,
							UAttributeSet_Stamina::GetStaminaRegenerationAttribute());
		ResetDelegateHandle(ResistanceChangeDelegate,
							OldAbilitySystemComponent,
							UAttributeSet_Resistance::GetResistanceAttribute());
		ResetDelegateHandle(BleedingChangeDelegate,
							OldAbilitySystemComponent,
							UAttributeSet_Health::GetBleedingAttribute());
		ResetDelegateHandle(BleedHealChangeDelegate,
							OldAbilitySystemComponent,
							UAttributeSet_Health::GetBleedHealingAttribute());

		ResetDelegateHandle(MaximumManaChangeDelegate,
							OldAbilitySystemComponent,
							UAttributeSet_Mana::GetMaximumManaAttribute());
		ResetDelegateHandle(CurrentManaChangeDelegate,
							OldAbilitySystemComponent,
							UAttributeSet_Mana::GetCurrentManaAttribute());

		ResetDelegateHandle(MaximumFocusChangeDelegate,
							OldAbilitySystemComponent,
							UAttributeSet_Mokong::GetTargetFocusAttribute());
		ResetDelegateHandle(CurrentFocusChangeDelegate,
							OldAbilitySystemComponent,
							UAttributeSet_Mokong::GetCurrentFocusAttribute());
	}

	bool bBindingDone = false;

	// Bind Health attribute delegates if the Ability System Component has the required Attribute Set -and- we are listening for Health attributes.
	if (bListenForAttributeChanges)
	{
		if (AbilitySystemComponent->HasAttributeSetForAttribute(UAttributeSet_Health::GetMaximumHealthAttribute()))
		{
			bBindingDone = true;

			MaximumHealthChangeDelegate = AbilitySystemComponent->
										  GetGameplayAttributeValueChangeDelegate(UAttributeSet_Health::GetMaximumHealthAttribute())
										  .AddUObject(this, &UPlayerHUDWidget::MaximumHealthChanged);
			CurrentHealthChangeDelegate = AbilitySystemComponent->
										  GetGameplayAttributeValueChangeDelegate(UAttributeSet_Health::GetCurrentHealthAttribute())
										  .AddUObject(this, &UPlayerHUDWidget::CurrentHealthChanged);
			HealthRegenerationChangeDelegate = AbilitySystemComponent->
											   GetGameplayAttributeValueChangeDelegate(UAttributeSet_Health::GetHealthRegenerationAttribute())
											   .AddUObject(this, &UPlayerHUDWidget::HealthRegenerationChanged);
			ResistanceChangeDelegate = AbilitySystemComponent->
									   GetGameplayAttributeValueChangeDelegate(UAttributeSet_Resistance::GetResistanceAttribute())
									   .AddUObject(this, &UPlayerHUDWidget::ResistanceChanged);
			BleedingChangeDelegate = AbilitySystemComponent->
									 GetGameplayAttributeValueChangeDelegate(UAttributeSet_Health::GetBleedingAttribute()).
									 AddUObject(this, &UPlayerHUDWidget::BleedingChanged);
			BleedHealChangeDelegate = AbilitySystemComponent->
									  GetGameplayAttributeValueChangeDelegate(UAttributeSet_Health::GetBleedHealingAttribute())
									  .AddUObject(this, &UPlayerHUDWidget::BleedingChanged);

			const float MaxHealth = AbilitySystemComponent->
					GetNumericAttribute(UAttributeSet_Health::GetMaximumHealthAttribute());
			const float CurrentHealth = AbilitySystemComponent->
					GetNumericAttribute(UAttributeSet_Health::GetCurrentHealthAttribute());

			// Call the Blueprint Events to initialize the values.
			On_MaximumHealthChanged(MaxHealth, 0.0f, CurrentHealth / MaxHealth);
			On_CurrentHealthChanged(CurrentHealth, 0.0f, CurrentHealth / MaxHealth);
			On_HealthRegenerationChanged(AbilitySystemComponent->
										 GetNumericAttribute(UAttributeSet_Health::GetHealthRegenerationAttribute()),
										 0.0f);

			BleedingChanged(FOnAttributeChangeData());
		}

		if (AbilitySystemComponent->HasAttributeSetForAttribute(UAttributeSet_Stamina::GetMaximumStaminaAttribute()))
		{
			bBindingDone = true;

			MaximumStaminaChangeDelegate = AbilitySystemComponent->
										   GetGameplayAttributeValueChangeDelegate(UAttributeSet_Stamina::GetMaximumStaminaAttribute())
										   .AddUObject(this, &UPlayerHUDWidget::MaximumStaminaChanged);
			CurrentStaminaChangeDelegate = AbilitySystemComponent->
										   GetGameplayAttributeValueChangeDelegate(UAttributeSet_Stamina::GetCurrentStaminaAttribute())
										   .AddUObject(this, &UPlayerHUDWidget::CurrentStaminaChanged);
			StaminaRegenerationChangeDelegate = AbilitySystemComponent->
												GetGameplayAttributeValueChangeDelegate(UAttributeSet_Stamina::GetStaminaRegenerationAttribute())
												.AddUObject(this, &UPlayerHUDWidget::StaminaRegenerationChanged);

			const float MaxStamina = AbilitySystemComponent->
					GetNumericAttribute(UAttributeSet_Stamina::GetMaximumStaminaAttribute());
			const float CurrentStamina = AbilitySystemComponent->
					GetNumericAttribute(UAttributeSet_Stamina::GetCurrentStaminaAttribute());

			// Call the Blueprint Events to initialize the values.
			On_MaximumStaminaChanged(MaxStamina, 0.0f, CurrentStamina / MaxStamina);
			On_CurrentStaminaChanged(CurrentStamina, 0.0f, CurrentStamina / MaxStamina);
			On_StaminaRegenerationChanged(AbilitySystemComponent->
										  GetNumericAttribute(UAttributeSet_Stamina::GetStaminaRegenerationAttribute()),
										  0.0f);
		}
		if (AbilitySystemComponent->HasAttributeSetForAttribute(UAttributeSet_Mana::GetMaximumManaAttribute()))
		{
			bBindingDone = true;

			MaximumManaChangeDelegate = AbilitySystemComponent->
										GetGameplayAttributeValueChangeDelegate(UAttributeSet_Mana::GetMaximumManaAttribute())
										.AddUObject(this, &UPlayerHUDWidget::MaximumManaChanged);
			CurrentManaChangeDelegate = AbilitySystemComponent->
										GetGameplayAttributeValueChangeDelegate(UAttributeSet_Mana::GetCurrentManaAttribute())
										.AddUObject(this, &UPlayerHUDWidget::CurrentManaChanged);

			const float MaxMana = AbilitySystemComponent->
					GetNumericAttribute(UAttributeSet_Mana::GetMaximumManaAttribute());
			const float CurrentMana = AbilitySystemComponent->
					GetNumericAttribute(UAttributeSet_Mana::GetCurrentManaAttribute());

			On_MaximumManaChanged(MaxMana, 0.0f, CurrentMana / MaxMana);
			On_CurrentManaChanged(CurrentMana, 0.0f, CurrentMana / MaxMana);

		}
		if (AbilitySystemComponent->HasAttributeSetForAttribute(UAttributeSet_Mokong::GetTargetFocusAttribute()))
		{
			bBindingDone = true;

			MaximumFocusChangeDelegate = AbilitySystemComponent->
										 GetGameplayAttributeValueChangeDelegate(UAttributeSet_Mokong::GetTargetFocusAttribute())
										 .AddUObject(this, &UPlayerHUDWidget::MaximumFocusChanged);

			CurrentFocusChangeDelegate = AbilitySystemComponent->
										 GetGameplayAttributeValueChangeDelegate(UAttributeSet_Mokong::GetCurrentFocusAttribute())
										 .AddUObject(this, &UPlayerHUDWidget::CurrentFocusChanged);

			const float MaxFocus = AbilitySystemComponent->
					GetNumericAttribute(UAttributeSet_Mokong::GetTargetFocusAttribute());
			const float CurrentFocus = AbilitySystemComponent->
					GetNumericAttribute(UAttributeSet_Mokong::GetCurrentFocusAttribute());

			On_MaximumFocusChanged(MaxFocus, 0.0f, CurrentFocus / MaxFocus);
			On_CurrentFocusChanged(CurrentFocus, 0.0f, CurrentFocus / MaxFocus);
		}
	}

	if (bListenForEffectEvents && AbilitySystemComponent
		.
		IsValid()
	)
	{
		bBindingDone = true;
		AbilitySystemComponent->OnMKGameplayEffectEventDelegate.AddDynamic(this,
																		   &UPlayerHUDWidget::
																		   OnGameplayEffectEventCallback);
	}

	K2_InitializeAbilitySystemWidget(bBindingDone);

	return bBindingDone;
}

UMKAbilitySystemComponent* UPlayerHUDWidget::GetOwnerAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void UPlayerHUDWidget::MaximumHealthChanged(const FOnAttributeChangeData& Data)
{
	const float CurrentHealth = AbilitySystemComponent->
			GetNumericAttribute(UAttributeSet_Health::GetCurrentHealthAttribute());

	On_MaximumHealthChanged(Data.NewValue, Data.OldValue, Data.NewValue > 0.f ? CurrentHealth / Data.NewValue : 0.f);
}

void UPlayerHUDWidget::CurrentHealthChanged(const FOnAttributeChangeData& Data)
{
	const float MaxHealth = AbilitySystemComponent->GetNumericAttribute(UAttributeSet_Health::GetMaximumHealthAttribute());

	On_CurrentHealthChanged(Data.NewValue, Data.OldValue, MaxHealth > 0.f ? Data.NewValue / MaxHealth : 0.f);
}

void UPlayerHUDWidget::HealthRegenerationChanged(const FOnAttributeChangeData& Data)
{
	On_HealthRegenerationChanged(Data.NewValue, Data.OldValue);

}

void UPlayerHUDWidget::ResistanceChanged(const FOnAttributeChangeData& Data)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	On_ResistanceChanged(AbilitySystemComponent->GetNumericAttributeBase(Data.Attribute), Data.NewValue);
}

void UPlayerHUDWidget::MaximumStaminaChanged(const FOnAttributeChangeData& Data)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}
	const float CurrentStamina = AbilitySystemComponent->
			GetNumericAttribute(UAttributeSet_Stamina::GetCurrentStaminaAttribute());

	On_MaximumStaminaChanged(Data.NewValue, Data.OldValue, Data.NewValue > 0.f ? CurrentStamina / Data.NewValue : 0.f);
}

void UPlayerHUDWidget::CurrentStaminaChanged(const FOnAttributeChangeData& Data)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}
	const float MaxStamina = AbilitySystemComponent->
			GetNumericAttribute(UAttributeSet_Stamina::GetMaximumStaminaAttribute());

	On_CurrentStaminaChanged(Data.NewValue, Data.OldValue, MaxStamina > 0.f ? Data.NewValue / MaxStamina : 0.f);
}

void UPlayerHUDWidget::StaminaRegenerationChanged(const FOnAttributeChangeData& Data)
{
	On_StaminaRegenerationChanged(Data.NewValue, Data.OldValue);
}

void UPlayerHUDWidget::MaximumManaChanged(const FOnAttributeChangeData& Data)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}
	const float CurrentMana = AbilitySystemComponent->GetNumericAttribute(UAttributeSet_Mana::GetCurrentManaAttribute());

	On_MaximumManaChanged(Data.NewValue, Data.OldValue, Data.NewValue > 0.f ? CurrentMana / Data.NewValue : 0.f);
}

void UPlayerHUDWidget::MaximumFocusChanged(const FOnAttributeChangeData& Data)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}
	const float CurrentFocus = AbilitySystemComponent->
			GetNumericAttribute(UAttributeSet_Mokong::GetCurrentFocusAttribute());

	On_MaximumFocusChanged(Data.NewValue, Data.OldValue, Data.NewValue > 0.f ? CurrentFocus / Data.NewValue : 0.f);
}

void UPlayerHUDWidget::CurrentManaChanged(const FOnAttributeChangeData& Data)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}
	const float MaxMana = AbilitySystemComponent->GetNumericAttribute(UAttributeSet_Mana::GetMaximumManaAttribute());

	On_CurrentManaChanged(Data.NewValue, Data.OldValue, MaxMana > 0.f ? Data.NewValue / MaxMana : 0.f);
}

void UPlayerHUDWidget::CurrentFocusChanged(const FOnAttributeChangeData& Data)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}
	const float MaxFocus = AbilitySystemComponent->GetNumericAttribute(UAttributeSet_Mokong::GetTargetFocusAttribute());

	On_CurrentFocusChanged(Data.NewValue, Data.OldValue, MaxFocus > 0.f ? Data.NewValue / MaxFocus : 0.f);
}

void UPlayerHUDWidget::BleedingChanged(const FOnAttributeChangeData& Data)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}
	const float Bleeding  = AbilitySystemComponent->GetNumericAttribute(UAttributeSet_Health::GetBleedingAttribute());
	const float BleedHeal = AbilitySystemComponent->GetNumericAttribute(UAttributeSet_Health::GetBleedHealingAttribute());

	const float Duration = BleedHeal > 0 ? Bleeding / BleedHeal : 0.f;

	On_BleedingChanged(Bleeding, BleedHeal, Duration);
}

void UPlayerHUDWidget::ResetDelegateHandle(FDelegateHandle           DelegateHandle,
										   UAbilitySystemComponent*  OldAbilitySystemComponent,
										   const FGameplayAttribute& Attribute)
{
	if (IsValid(OldAbilitySystemComponent))
	{
		OldAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).Remove(DelegateHandle);
		DelegateHandle.Reset();
	}
}

void UPlayerHUDWidget::OnGameplayEffectEventCallback(const FActiveGameplayEffect& Effect,
													 const EASEffectEventType     EventType)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	FGameplayTagContainer AssetTags{};
	Effect.Spec.GetAllAssetTags(AssetTags);

	if (!EffectEventTagRequirements.RequirementsMet(AssetTags))
	{
		return;
	}

	if (!UMKGameplayEffectUIData::GetGameplayEffectUIDataFromActiveEffect(Effect))
	{
		return;
	}

	FMKEffectEventInfo Info{};
	Info.bIsInhibited = Effect.bIsInhibited;
	Info.Spec         = Effect.Spec;
	Info.Def          = Effect.Spec.Def;
	Info.ActiveEffect = Effect;

	K2_OnGameplayEffectEventCallback(AbilitySystemComponent.Get(), EventType, Effect.Handle, Info);
}
