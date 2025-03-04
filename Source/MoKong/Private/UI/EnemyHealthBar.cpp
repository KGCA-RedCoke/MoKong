// CopyRight KGCA - Team RedCoke


#include "UI/EnemyHealthBar.h"

#include "Ability/MKAbilitySystemComponent.h"
#include "AttributeSets/AttributeSet_Health.h"
#include "AttributeSets/AttributeSet_Resistance.h"

bool UEnemyHealthBar::InitializeAbilitySystemWidget(UAbilitySystemComponent* OwnerAbilitySystemComponent)
{
	UAbilitySystemComponent* OldAbilitySystemComponent = AbilitySystemComponent.Get();

	bool bBindingDone = Super::InitializeAbilitySystemWidget(OwnerAbilitySystemComponent);

	if (!bBindingDone)
	{
		return false;
	}

	if (IsValid(OldAbilitySystemComponent))
	{
		ResetDelegateHandle(MaximumHealthChangeDelegate,
							OldAbilitySystemComponent,
							UAttributeSet_Health::GetMaximumHealthAttribute());
		ResetDelegateHandle(CurrentHealthChangeDelegate,
							OldAbilitySystemComponent,
							UAttributeSet_Health::GetCurrentHealthAttribute());
		ResetDelegateHandle(ResistanceChangeDelegate,
							OldAbilitySystemComponent,
							UAttributeSet_Resistance::GetResistanceAttribute());
	}

	if (bListenForAttributeChanges)

		if (AbilitySystemComponent->HasAttributeSetForAttribute(UAttributeSet_Health::GetMaximumHealthAttribute()))
		{
			bBindingDone = true;

			MaximumHealthChangeDelegate = AbilitySystemComponent->
										  GetGameplayAttributeValueChangeDelegate(UAttributeSet_Health::GetMaximumHealthAttribute())
										  .AddUObject(this, &UEnemyHealthBar::MaximumHealthChanged);
			CurrentHealthChangeDelegate = AbilitySystemComponent->
										  GetGameplayAttributeValueChangeDelegate(UAttributeSet_Health::GetCurrentHealthAttribute())
										  .AddUObject(this, &UEnemyHealthBar::CurrentHealthChanged);
			ResistanceChangeDelegate = AbilitySystemComponent->
									   GetGameplayAttributeValueChangeDelegate(UAttributeSet_Resistance::GetResistanceAttribute())
									   .AddUObject(this, &UEnemyHealthBar::ResistanceChanged);

			const float MaxHealth = AbilitySystemComponent->
					GetNumericAttribute(UAttributeSet_Health::GetMaximumHealthAttribute());
			const float CurrentHealth = AbilitySystemComponent->
					GetNumericAttribute(UAttributeSet_Health::GetCurrentHealthAttribute());

			// Call the Blueprint Events to initialize the values.
			On_MaximumHealthChanged(MaxHealth, 0.0f, CurrentHealth / MaxHealth);
			On_CurrentHealthChanged(CurrentHealth, 0.0f, CurrentHealth / MaxHealth);
			On_ResistanceChanged(AbilitySystemComponent->
								 GetNumericAttributeBase(UAttributeSet_Resistance::GetResistanceAttribute()),
								 AbilitySystemComponent->
								 GetNumericAttribute(UAttributeSet_Resistance::GetResistanceAttribute()));
		}

	return bBindingDone;
}

void UEnemyHealthBar::MaximumHealthChanged(const FOnAttributeChangeData& Data)
{
	const float CurrentHealth = AbilitySystemComponent->
			GetNumericAttribute(UAttributeSet_Health::GetCurrentHealthAttribute());

	On_MaximumHealthChanged(Data.NewValue, Data.OldValue, Data.NewValue > 0.f ? CurrentHealth / Data.NewValue : 0.f);
}

void UEnemyHealthBar::CurrentHealthChanged(const FOnAttributeChangeData& Data)
{
	const float MaxHealth = AbilitySystemComponent->GetNumericAttribute(UAttributeSet_Health::GetMaximumHealthAttribute());

	On_CurrentHealthChanged(Data.NewValue, Data.OldValue, MaxHealth > 0.f ? Data.NewValue / MaxHealth : 0.f);
}

void UEnemyHealthBar::ResistanceChanged(const FOnAttributeChangeData& Data)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	On_ResistanceChanged(AbilitySystemComponent->GetNumericAttributeBase(Data.Attribute), Data.NewValue);
}
