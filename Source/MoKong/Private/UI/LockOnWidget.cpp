// CopyRight KGCA - Team RedCoke


#include "UI/LockOnWidget.h"
#include "Animation/WidgetAnimation.h"

void ULockOnWidget::ShowWidget(bool bShow)
{
	PlayAnimation(
				  LockOnAnimation.Get(),
				  0,
				  1,
				  bShow ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse
				 );
}
