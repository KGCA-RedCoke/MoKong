// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "SkillSystemTypes.h"
#include "Components/ActorComponent.h"
#include "SkillTree.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SKILLSYSTEM_API USkillTree : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillTree();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float                        DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SkillTree|NOEdit|General")
	TArray<FSkillDatabase> UnlockedSkills;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SkillTree|NOEdit|General")
	FSkillData CurrentSkillData;

	// UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SkillTree|NOEdit")
	
	
};
