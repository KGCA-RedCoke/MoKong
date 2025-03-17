// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Actor/MKAbilityPlayerController.h"
#include "MKPlayerController.generated.h"

class AMokongEnemy;
class UInventorySystemComponent;
class UPlayerHUDWidget;
/**
 * 
 */
UCLASS()
class MOKONG_API AMKPlayerController : public AMKAbilityPlayerController
{
	GENERATED_BODY()

public:
	AMKPlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category= "Mokong|Player|Controller")
	void ResetState();

	UFUNCTION(BlueprintCallable)
	void AddCombatEnemy(AMokongEnemy* Enemy);

	UFUNCTION(BlueprintCallable)
	void RemoveCombatEnemy(AMokongEnemy* Enemy);

	UFUNCTION(BlueprintCallable)
	void RemoveAllCombatEnemies()
	{
		CurrentCombatEnemies.Empty();
	}

	UFUNCTION(BlueprintCallable)
	AMokongEnemy* GetEnemy(AMokongEnemy* Enemy);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, Category= "Mokong|Player|Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerHUDWidget> PlayerAttributeWidget;

	UPROPERTY(BlueprintReadWrite, Category= "Mokong|Player|Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventorySystemComponent> PlayerInventory;

private:
	/** 현재 전투중인 적 리스트 */
	UPROPERTY(BlueprintReadWrite, Category= "Mokong|Player|State", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<AMokongEnemy>> CurrentCombatEnemies;
};
