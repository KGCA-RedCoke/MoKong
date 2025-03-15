// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ItemTypes.h"
#include "InventoryPanelButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedButtonPanel, const EInventoryPanel, Panel);



/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventoryPanelButton : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:
	void RefreshPanelButton(const EInventoryPanel ActivePanel) const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FText GetPanelName() const { return PanelName; }

protected:
	UFUNCTION()
	void OnClickedEvent();

public:
	FOnClickedButtonPanel OnClickedButtonPanel;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UBorder* Border;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Inventory|Panel")
	EInventoryPanel Panel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Inventory|Panel")
	FButtonStyle Off_ButtonStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Inventory|Panel")
	FButtonStyle On_ButtonStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Inventory|Panel")
	FText PanelName;
};
