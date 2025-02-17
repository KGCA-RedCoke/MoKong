// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CatchUpBarBase.generated.h"

class UImage;
class UProgressBar;
class USizeBox;

UENUM(BlueprintType)
enum class ECatchupBarState : uint8
{
	Paused,
	CatchingUp,
	Caught
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMainPercentChanged, float, NewPercent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCatchupPercentChanged, float, NewPercent);

/**
 * 
 */
UCLASS()
class UISYSTEM_API UCatchUpBarBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void InitializeCatchupValues(float InCurrentValue, float InMaxValue);
	UFUNCTION(BlueprintCallable)
	void SetCurrentValue(float NewValue, float OldValue);
	UFUNCTION(BlueprintCallable)
	void SetMaxValue(float NewValue, float OldValue);
	UFUNCTION(BlueprintCallable)
	void ReCalculatePercent(float Delta);
	UFUNCTION(BlueprintCallable)
	void SetCatchupPercent(float NewPercent);
	UFUNCTION(BlueprintCallable)
	void StartCatchingUp();

public:
	UPROPERTY(BlueprintAssignable, Category = "CatchupBar")
	FOnMainPercentChanged OnMainPercentChanged;

	UPROPERTY(BlueprintAssignable, Category = "CatchupBar")
	FOnCatchupPercentChanged OnCatchupPercentChanged;

private:
	UFUNCTION()
	void UpdateMainPercent(float NewPercent);
	UFUNCTION()
	void UpdateCatchupPercent(float NewPercent);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CatchupBar|Widget", Meta = (BindWidget))
	TObjectPtr<UProgressBar> MainProgressBar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CatchupBar|Widget", Meta = (BindWidget))
	TObjectPtr<UProgressBar> CatchupProgressBar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CatchupBar|Widget", Meta = (BindWidget))
	TObjectPtr<UImage> CurrentProgressHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CatchupBar|Settings")
	FLinearColor MainColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CatchupBar|Settings")
	FLinearColor CatchupColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CatchupBar")
	ECatchupBarState State;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CatchupBar")
	float CurrentValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CatchupBar")
	float MaxValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CatchupBar")
	float MainPercent = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CatchupBar")
	float CatchupPercent = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CatchupBar")
	FTimerHandle WaitingTimerHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CatchupBar")
	float CatchupWaitTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CatchupBar")
	float CatchupSpeed;
};
