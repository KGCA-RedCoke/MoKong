// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ItemTypes.h"
#include "PanelSortButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonClicked, const ESortMethod, SortMethod);

/**
 * Sort Button for Inventory Panel
 */
UCLASS()
class INVENTORYSYSTEM_API UPanelSortButton : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetBorderColorWhite();

	UFUNCTION()
	void SetBorderColorTransparent();

	UFUNCTION()
	void OnClickedEvent();

public:
	FOnButtonClicked OnButtonClicked;

protected:
#pragma region Widget
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCommonBorder> Border;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCommonTextBlock> Title_Text;
#pragma	endregion

#pragma region Data
	UPROPERTY(EditInstanceOnly)
	ESortMethod SortMethod;

	UPROPERTY(EditInstanceOnly)
	FText Title;
#pragma endregion
};
