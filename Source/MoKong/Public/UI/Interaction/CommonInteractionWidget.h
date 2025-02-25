// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommonInteractionWidget.generated.h"

class USizeBox;
class UBorder;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class MOKONG_API UCommonInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:


private:
	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UWidgetSwitcher> ContentSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<USizeBox> Container;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta = (BindWidget, AllowPrivateAccess))
	TObjectPtr<UBorder> Icon;

};
