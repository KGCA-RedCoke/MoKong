// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "MKAbilitySystemData.generated.h"

class UMKAbilitySystemComponent;
struct FGameplayAbilitySpecHandle;
class UMKGameplayAbility;
class UGameplayEffect;
class UGameplayAbility;

USTRUCT(Blueprintable)
struct FMKAttributeMaxValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat MaxFloat{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttribute MaxAttribute{};
};


USTRUCT(Blueprintable)
struct FMKAttributeInitializer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat Value;
};

// Contains data used to initialize an Ability System Component.
USTRUCT(BlueprintType)
struct FAbilitySystemInitializationData
{
	GENERATED_BODY()

	// An array of Attribute Sets to create.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSet<TSubclassOf<UAttributeSet>> AttributeSets;

	// A map of Attributes / float used to set base values.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<FGameplayAttribute, FMKAttributeInitializer> AttributeBaseValues;

	// An Array of Gameplay Abilities to give.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSet<TSubclassOf<UGameplayAbility>> GameplayAbilities;

	// An array of Gameplay Effects to apply.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSet<TSubclassOf<UGameplayEffect>> GameplayEffects;

	// A container of GameplayTags to apply.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTagContainer GameplayTags;
};

// Returns the value of an Attribute based on the search type.
UENUM(BlueprintType)
enum class EAttributeSearchType : uint8
{
	// Returns the final value of the Attribute including all stateful Gameplay Effect modifiers.
	FinalValue,

	// Returns the base value of the Attribute. (Excludes duration based Gameplay Effect modifiers)
	BaseValue,

	// Returns the Final Value minus the Base Value.
	BonusValue
};

UENUM(BlueprintType)
enum class EHitReactDirection : uint8
{
	None,
	Forward,
	Left,
	Right,
	Backward
};

UENUM(BlueprintType)
enum class EMKAbilityActivationType: uint8
{
	OnInputTriggered,
	WhileInputActive
};

USTRUCT(BlueprintType)
struct FMKGameplayEffectContainer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayEffectContainer")
	TSubclassOf<class UMKTargetType> TargetType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayEffectContainer")
	TArray<TSubclassOf<UGameplayEffect>> TargetGameplayEffectClasses;
};

USTRUCT(BlueprintType)
struct FMKGameplayEffectContainerSpec
{
	GENERATED_BODY()

public:
	/** Computed target data */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	FGameplayAbilityTargetDataHandle TargetData;

	/** List of gameplay effects to apply to the targets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffectContainer)
	TArray<FGameplayEffectSpecHandle> TargetGameplayEffectSpecs;

	/** Returns true if this has any valid effect specs */
	bool HasValidEffects() const;

	/** Returns true if this has any valid targets */
	bool HasValidTargets() const;

	/** Adds new targets to target data */
	void AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);
};

USTRUCT(BlueprintType)
struct FMKAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMKGameplayAbility> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FMKAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);

	UPROPERTY(BlueprintReadOnly)
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};


UCLASS()
class ABILITYSYSTEM_API UMKAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:
	// Grants the ability set to the specified ability system component.
	// The returned handles can be used later to take away anything that was granted.
	void GiveToAbilitySystem(UMKAbilitySystemComponent* InASC, FMKAbilitySet_GrantedHandles* OutGrantedHandles,
							 UObject*                   SourceObject = nullptr) const;

protected:
	// Gameplay abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta=(TitleProperty=Ability))
	TArray<FMKAbilitySet_GameplayAbility> GrantedGameplayAbilities;
};
