// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Components/ActorComponent.h"
#include "InteractionManager.generated.h"


class UPostProcessComponent;
class UInteractionWidget;
class UInteractionTargetComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class UISYSTEM_API UInteractionManager : public UActorComponent
{
	GENERATED_BODY()

/*#pragma region Variables

protected:
#pragma region Data

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	TArray<TObjectPtr<UInteractionTargetComponent>> InteractionTargets;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	TArray<TObjectPtr<UInteractionTargetComponent>> PointsOfInterests;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	TArray<TObjectPtr<UInteractionTargetComponent>> PendingTargets;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	TArray<TObjectPtr<UInteractionTargetComponent>> DeactivatedTargets;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	TArray<TObjectPtr<UInteractionWidget>> WidgetPool;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	TObjectPtr<APlayerController> OwnerController;

	UPROPERTY(BlueprintReadWrite, Category = "Data")
	TObjectPtr<UPostProcessComponent> PostProcessComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	TObjectPtr<UInteractionWidget> CurrentInteractionWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	TObjectPtr<UInteractionWidget> BestInteractionTarget;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	TArray<FKey> InteractionKeys;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	float CurrentHoldTime;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	float RepeatCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float PendingTargetCheckInterval = .35f;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	int RepeatedTime;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	bool bIsInteracting;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	bool bKeyJustPressed;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	bool bIsGamepad;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	FKey LastPressedKey;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	FTimerHandle BeginUpdateKeys_TimerHandle;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	FTimerHandle PendingTarget_TimerHandle;

	UPROPERTY(BlueprintReadWrite, Category = "Data")
	UMaterialInstanceDynamic* OutlineInstanceDynamic_Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInterface* Outline_Material;

#pragma endregion Data

#pragma region Main
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Main")
	int DefaultObjectPoolSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main")
	TSubclassOf<UInteractionWidget> IndicatorWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main")
	TObjectPtr<UInputAction> InteractionInputAction;
#pragma endregion Main

#pragma region Debug

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bDebugMode;

#pragma endregion Debug

#pragma region MarkerSettings

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MarkerSettings")
	FMargin WidgetScreenMargin{125.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MarkerSettings")
	float ScreenRadiusProgress{.5f};

#pragma endregion MarkerSettings

#pragma endregion Variables*/
};
