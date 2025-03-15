// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Actor/Weapon.h"
#include "WK_Staff.generated.h"

UCLASS()
class MOKONG_API AWK_Staff : public AWeapon
{
	GENERATED_BODY()

public:
	AWK_Staff();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHitActorAdded(FHitResult LastItem);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Ability|Effect", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> ChargeFocusEffect;	// 공격 성공시 충전되는 포커스 이펙트

};
