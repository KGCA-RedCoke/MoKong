// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "MKAbilityCharacter.h"
#include "MKAbilityPlayer.generated.h"

class UMKInputConfig;

UCLASS(Blueprintable)
class ABILITYSYSTEM_API AMKAbilityPlayer : public AMKAbilityCharacter
{
	GENERATED_BODY()

public:
	AMKAbilityPlayer();

protected:
	// ------------------ Begin Ability System ------------------
	/** ASC를 가져오고 초기화하자 */
	virtual void InitializeAbilitySystem();
	/** (서버에서 호출) - 컨트롤러가 이 캐릭터를 소유할 때 호출 */
	virtual void PossessedBy(AController* NewController) override;
	/** (클라이언트에서 호출) - PlayerState가 서버로 Replicate(복제)될 때 호출 */
	virtual void OnRep_PlayerState() override;
	// ------------------ End Ability System ------------------

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void InputAbilityInputTagPressed(FGameplayTag InputTag);
	void InputAbilityInputTagReleased(FGameplayTag InputTag);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem|Input")
	UMKInputConfig* InputConfig;
};
