// CopyRight KGCA - Team RedCoke


#include "Actor/Gourd.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AGourd::AGourd()
{
	SK_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GourdSK_Mesh"));
	SK_Mesh->SetupAttachment(GetRootComponent());

	SetActorEnableCollision(false);
}

void AGourd::BeginPlay()
{
	Super::BeginPlay();
}

void AGourd::InitializeData()
{
	Super::InitializeData();

	AbilitySystemComponent =
			UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(AbilitySystemComponent);

	if (auto* CastPtr = static_cast<FMKGourdItemSpec*>(ItemSpecPtr))
	{
		GourdData = *CastPtr;

		Remain = GourdData.Remain;

		SK_Mesh->SetSkeletalMesh(GourdData.SK_Mesh.LoadSynchronous());
	}
}

bool AGourd::CanConsume() const
{
	return Remain > 0;
}

void AGourd::SwitchData(FName RowName)
{
	ItemData.RowName = RowName;

	ItemSpecPtr = ItemData.GetRow<FMKGourdItemSpec>("Cannot find row data");
	if (ItemSpecPtr)
	{
		GourdData = *static_cast<FMKGourdItemSpec*>(ItemSpecPtr);

		Remain = GourdData.Remain;

		SK_Mesh->SetSkeletalMesh(GourdData.SK_Mesh.LoadSynchronous());
	}
}

void AGourd::ConsumeGourd()
{
	Remain--;

	for (auto& GE : GourdData.AssociatedGE)
	{
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GE.LoadSynchronous(),
				 GourdLevel,
				 AbilitySystemComponent->MakeEffectContext());

		if (SpecHandle.IsValid())
		{
			FGameplayTag          HealTag = FGameplayTag::RequestGameplayTag(FName("Buff.Heal"));
			FGameplayTagContainer TagContainer;
			SpecHandle.Data->GetAllAssetTags(TagContainer);
			if (TagContainer.HasTag(HealTag))
			{
				SpecHandle.Data->
						   SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Buff.Heal.GourdAmount")),
												   GourdData.HealAmount);
			}

			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}

	}
}
