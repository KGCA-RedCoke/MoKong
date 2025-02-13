// CopyRight KGCA - Team RedCoke


#include "Widgets/EffectWidgetController.h"

#include "AbilitySystemComponent.h"
#include "Ability/MKGameplayEffectUIData.h"
#include "Widgets/EffectWidget.h"

class UWorld* UEffectWidgetController::GetWorld() const
{
	if (GetOuter())
	{
		return GetOuter()->GetWorld();
	}

	return nullptr;
}

ETriggerUpdateStatus UEffectWidgetController::TriggerUpdate()
{
	if (!OwningPlayer || !Widget)
	{
		return ETriggerUpdateStatus::NotValid;
	}

	if (!ActiveEffectHandle.IsValid())
	{
		RemoveEffect();
		return ETriggerUpdateStatus::NotValid;
	}

	UpdateEffectFromHandle();

	if (!EffectData || EffectData->IsPendingRemove)
	{
		RemoveEffect();
		return ETriggerUpdateStatus::NotValid;
	}

	UpdateFields();

	return K2_TriggerUpdate();
}

void UEffectWidgetController::InitializeController_Implementation(APlayerController*                   InOwningPlayer,
																  UPanelWidget*                        InPanelWidget,
																  FActiveGameplayEffectHandle          Handle,
																  const UMKGameplayEffectUIData* const InEffectUIData)
{
	if (!InPanelWidget || !InOwningPlayer || !Handle.IsValid() || !InEffectUIData)
	{
		return;
	}

	PanelWidget        = InPanelWidget;
	OwningPlayer       = InOwningPlayer;
	EffectUIData       = InEffectUIData;
	ActiveEffectHandle = Handle;

	UpdateEffectFromHandle();

	if (!EffectData || EffectData->IsPendingRemove)
	{
		RemoveEffect();
		return;
	}

	if (!Handle.IsValid())
	{
		return;
	}

	UAbilitySystemComponent* const ASC = Handle.GetOwningAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	// Subscribe to all available events on the gameplay effect event set, so that the controller can respond accordingly
	FActiveGameplayEffectEvents* const EventSet = ASC->GetActiveEffectEventSet(Handle);
	EventSet->OnEffectRemoved.AddUObject(this, &ThisClass::OnEffectRemoved);
	EventSet->OnStackChanged.AddUObject(this, &ThisClass::OnStackChanged);
	EventSet->OnInhibitionChanged.AddUObject(this, &ThisClass::OnInhibitionChanged);
	EventSet->OnTimeChanged.AddUObject(this, &ThisClass::OnTimeChanged);

	UpdateFields();
}

TSoftObjectPtr<UTexture2D> UEffectWidgetController::ExtractIcon_Implementation()
{
	return EffectUIData ? EffectUIData->IconTexture : nullptr;
}

FText UEffectWidgetController::ExtractDescription_Implementation()
{
	return EffectUIData->Description;
}

FText UEffectWidgetController::ExtractTitle_Implementation()
{
	return EffectUIData->Title;
}

float UEffectWidgetController::CalculateDuration_Implementation(float NewDuration)
{
	if (NewDuration > -1.f)
	{
		return NewDuration;
	}
	return EffectData ? EffectData->GetDuration() : -1.f;
}

int32 UEffectWidgetController::CalculateStackCount_Implementation(int32 NewStackCount)
{
	if (NewStackCount > -1)
	{
		return NewStackCount;
	}
	return EffectData ? EffectData->ClientCachedStackCount : 0;
}

void UEffectWidgetController::StartDurationUpdate()
{
	if (!Widget)
	{
		return;
	}

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	TimerManager.SetTimer(DurationUpdateTimer, this, &ThisClass::UpdateDurationOnWidget, DurationUpdateFrequency, true);
}

void UEffectWidgetController::UpdateDurationOnWidget()
{
	if (!Widget)
	{
		GetWorld()->GetTimerManager().ClearTimer(DurationUpdateTimer);
		return;
	}

	UpdateEffectFromHandle();
	const float RemainingDuration = EffectData->GetTimeRemaining(GetWorld()->GetTimeSeconds());

	Widget->UpdateDuration(FMath::Max(RemainingDuration, 0.f));

	if (RemainingDuration <= 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(DurationUpdateTimer);
	}
}

void UEffectWidgetController::UpdateFields()
{
	if (!EffectData)
	{
		return;
	}

	OnTimeChanged(ActiveEffectHandle, EffectData->StartServerWorldTime, EffectData->GetDuration());
	OnStackChanged(ActiveEffectHandle, EffectData->ClientCachedStackCount, 0);

	if (UE_MVVM_SET_PROPERTY_VALUE(Title, ExtractTitle()))
	{
		if (Widget)
		{
			Widget->UpdateTitle(Title);
		}
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(Title);
	}

	if (UE_MVVM_SET_PROPERTY_VALUE(Description, ExtractDescription()))
	{
		if (Widget)
		{
			Widget->UpdateDescription(Description);
		}
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(Description);
	}

	if (UE_MVVM_SET_PROPERTY_VALUE(SoftIcon, ExtractIcon()) || UE_MVVM_SET_PROPERTY_VALUE(SoftMaterial,
			 ExtractSoftMaterial()))
	{
		if (Widget)
		{
			Widget->UpdateIcon(SoftIcon, SoftMaterial);
		}
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(SoftIcon);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(SoftMaterial);
	}
}

void UEffectWidgetController::UpdateEffectFromHandle()
{
	const UAbilitySystemComponent* ASC = ActiveEffectHandle.GetOwningAbilitySystemComponent();

	if (!ASC)
	{
		return;
	}

	EffectData = ASC->GetActiveGameplayEffect(ActiveEffectHandle);
}

void UEffectWidgetController::OnEffectRemoved(const FGameplayEffectRemovalInfo& GameplayEffectRemovalInfo)
{
	K0_OnEffectRemoved(GameplayEffectRemovalInfo);
	RemoveEffect();
}

void UEffectWidgetController::OnStackChanged(FActiveGameplayEffectHandle ActiveGameplayEffectHandle,
											 const int32                 NewStackCount, const int32 OldStackCount)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentStacks, CalculateStackCount(NewStackCount)))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(CurrentStacks);
		K0_OnStackChanged(NewStackCount, OldStackCount);
	}
}

void UEffectWidgetController::OnInhibitionChanged(FActiveGameplayEffectHandle ActiveGameplayEffectHandle,
												  const bool                  bIsInhibited)
{}

void UEffectWidgetController::OnTimeChanged(FActiveGameplayEffectHandle ActiveGameplayEffectHandle,
											const float                 NewStartTime, const float NewDuration)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(TotalDuration, CalculateDuration(NewDuration)))
	{
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(TotalDuration);
		K0_OnTimeChanged(NewStartTime, TotalDuration);

		if (TotalDuration > 1.f)
		{
			StartDurationUpdate();
		}
	}
}

void UEffectWidgetController::K0_OnEffectRemoved_Implementation(
	const FGameplayEffectRemovalInfo& GameplayEffectRemovalInfo)
{}

void UEffectWidgetController::K0_OnStackChanged_Implementation(const int32 NewStackCount, const int32 OldStackCount)
{
	if (!Widget)
	{
		return;
	}
	Widget->UpdateStackCount(CurrentStacks);
}

void UEffectWidgetController::K0_OnInhibitionChanged_Implementation(const bool bIsInhibited)
{}

void UEffectWidgetController::K0_OnTimeChanged_Implementation(const float NewStartTime, const float NewDuration)
{
	if (!Widget)
	{
		return;
	}
	Widget->UpdateDuration(TotalDuration);
}

void UEffectWidgetController::RemoveEffect()
{
	if (Widget)
	{
		Widget->RemoveFromParent();
		Widget = nullptr;
	}
	K2_RemoveEffect();
	MarkAsGarbage();
}

TSoftObjectPtr<UMaterialInterface> UEffectWidgetController::ExtractSoftMaterial_Implementation()
{
	return EffectUIData ? EffectUIData->IconMaterial : nullptr;
}

ETriggerUpdateStatus UEffectWidgetController::K2_TriggerUpdate_Implementation()
{
	return ETriggerUpdateStatus::UpdateOK;
}
