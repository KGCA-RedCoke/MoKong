// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_ComboWindow.generated.h"

class UCombatSystem;
/**
 * 
 */
UCLASS()
class ABILITYSYSTEM_API UAnimNotifyState_ComboWindow : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_ComboWindow();

protected:
	virtual void NotifyBegin(USkeletalMeshComponent*          MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
							 const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent*          MeshComp, UAnimSequenceBase* Animation,
						   const FAnimNotifyEventReference& EventReference) override;

	virtual FString GetNotifyName_Implementation() const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextComboSection;

	TWeakObjectPtr<AActor>        CachedOwner;
	TWeakObjectPtr<UCombatSystem> CombatSystem;
};
