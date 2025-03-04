// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Actor/MKAbilityNPC.h"
#include "AI/Interface/EnemyAIInterface.h"
#include "WukongSummoned.generated.h"

UCLASS()
class MOKONG_API AWukongSummoned : public AMKAbilityNPC, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWukongSummoned();

public:
	//~~ IGenericTeamAgentInterface Begin ~~//
	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(static_cast<uint8>(TeamID)); }
	//~~ IGenericTeamAgentInterface End ~~//

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Player|Setup",
		DisplayName = "팀",
		meta = (AllowprivateAccess = "true"))
	ETeamType TeamID;
};
