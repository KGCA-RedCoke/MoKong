// CopyRight KGCA - Team RedCoke


#include "Widgets/EffectWidget_Skill.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"

void UEffectWidget_Skill::UpdateTitle(const FText NewTitle)
{}

void UEffectWidget_Skill::UpdateDescription(const FText NewDescription)
{}

void UEffectWidget_Skill::UpdateIcon(TSoftObjectPtr<UTexture2D>         NewSoftTexture,
									 TSoftObjectPtr<UMaterialInterface> SoftMaterial)
{
	if (!SoftMaterial.IsNull())
	{
		Image_Icon->SetBrushFromSoftMaterial(SoftMaterial);
	}
	else if (!NewSoftTexture.IsNull())
	{
		Image_Icon->SetBrushFromSoftTexture(NewSoftTexture);
	}


}

void UEffectWidget_Skill::UpdateStackCount(const int32 NewStackCount)
{}

void UEffectWidget_Skill::UpdateDuration(const float NewDuration)
{
	if (!GetWorld())
	{
		return;
	}


}

void UEffectWidget_Skill::SetCooldown(const float Duration)
{
	if (Duration <= 0.f)
	{
		return;
	}

	CooldownDuration = Duration; // 전체 쿨다운 시간 저장
	ElapsedTime      = 0.f; // 경과 시간 초기화

	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle,
										   this,
										   &UEffectWidget_Skill::OnCooldownFinished,
										   TickInterval,
										   true);
}

void UEffectWidget_Skill::OnCooldownFinished()
{
	if (!ProgressBar_CoolTime)
	{
		return;
	}

	ElapsedTime += TickInterval; // 타이머가 0.05초마다 호출되므로 시간 추가

	float Progress = FMath::Clamp(1.f - (ElapsedTime / CooldownDuration), 0.f, 1.f);
	ProgressBar_CoolTime->SetPercent(Progress);

	// 쿨다운 종료 시 타이머 정지
	if (ElapsedTime >= CooldownDuration)
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	}
}
