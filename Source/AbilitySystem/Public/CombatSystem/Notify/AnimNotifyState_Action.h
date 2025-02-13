// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CombatSystem/AttackSystemTypes.h"
#include "AnimNotifyState_Action.generated.h"

class UCombatSystem;
/**
 * 
 */
UCLASS(DisplayName= "몽타주 상태 지정")
class ABILITYSYSTEM_API UAnimNotifyState_Action : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	virtual void NotifyBegin(USkeletalMeshComponent*          MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
							 const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent*          MeshComp, UAnimSequenceBase* Animation,
						   const FAnimNotifyEventReference& EventReference) override;
	virtual FString GetNotifyName_Implementation() const override;

private:
	bool CheckComponentIsValid(USkeletalMeshComponent* MeshComp);

protected:
	UPROPERTY(EditInstanceOnly)
	ECombatState ActionState;
	UPROPERTY(EditInstanceOnly)
	ECombatState NextActionState;

	TWeakObjectPtr<AActor>        CachedOwner;
	TWeakObjectPtr<UCombatSystem> CombatSystem;

};
