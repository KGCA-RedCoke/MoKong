// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockOnWidget.generated.h"

class UCanvasPanel;
class UImage;
/**
 * 
 */
UCLASS()
class MOKONG_API ULockOnWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowWidget(bool bShow);

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget), Category = "LockOn")
	TObjectPtr<UCanvasPanel> LockOnCanvasPanel;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget), Category = "LockOn")
	TObjectPtr<UImage> LockOnImage;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget), Category = "LockOn")
	TObjectPtr<UImage> LockOnBackgroundImage;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidgetAnim), Category = "LockOn", Transient)
	TObjectPtr<UWidgetAnimation> LockOnAnimation;
};
