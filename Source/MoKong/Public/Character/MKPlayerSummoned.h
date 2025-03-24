// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Actor/MKAbilityNPC.h"
#include "AI/Interface/EnemyAIInterface.h"
#include "MKPlayerSummoned.generated.h"

class UPoseableMeshComponent;

UCLASS()
class MOKONG_API AMKPlayerSummoned : public AMKAbilityNPC, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AMKPlayerSummoned();

protected:
	virtual void BeginPlay() override;

	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(static_cast<uint8>(TeamID)); }

public:
	UFUNCTION(BlueprintCallable)
	void ReplicateMeshFromPlayer();

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	float LifeTimeLevel = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Modular|Parts", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	TObjectPtr<USkeletalMesh> HelmetMesh;
	UPROPERTY(BlueprintReadWrite, Category = "Modular|Parts", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	TObjectPtr<USkeletalMesh> SuitMesh;
	UPROPERTY(BlueprintReadWrite, Category = "Modular|Parts", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	TObjectPtr<USkeletalMesh> GlovesMesh;
	UPROPERTY(BlueprintReadWrite, Category = "Modular|Parts", meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	TObjectPtr<USkeletalMesh> ShoesMesh;

	UPROPERTY(BlueprintReadOnly, Category = "Modular|Parts", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UAnimMontage> SummonedMontage;
	UPROPERTY(BlueprintReadOnly, Category = "Modular|Parts", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UAnimMontage> DisappearMontage;

	UPROPERTY(EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Player|Setup",
		DisplayName = "팀",
		meta = (AllowprivateAccess = "true"))
	ETeamType TeamID;
};
