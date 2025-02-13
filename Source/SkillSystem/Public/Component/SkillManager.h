// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "SkillSystemTypes.h"
#include "Components/ActorComponent.h"
#include "SkillManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SKILLSYSTEM_API USkillManager : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillManager();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Skill")
	int GetSkillPoints() const { return SkillPoints; }

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void SetSkillPoints(int NewSkillPoints) { SkillPoints = NewSkillPoints; }

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void AddSkillPoints(int AddedSkillPoints) { SkillPoints += AddedSkillPoints; }

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SkillTree")
	TArray<FSkillDatabase> UnlockedSkills;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "SkillTree")
	FSkillData CurrentSkillData;

	UPROPERTY(BlueprintReadOnly)
	int SkillPoints = 0;	// 사용 가능한 스킬 포인트


};
