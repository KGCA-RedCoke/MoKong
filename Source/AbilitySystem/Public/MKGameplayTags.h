// CopyRight KGCA - Team RedCoke

#pragma once

#include "CoreMinimal.h"
#include "Runtime/GameplayTags/Public/NativeGameplayTags.h"

namespace MoKong
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_NoLevel);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_Type_Cooldown);

	namespace CharacterTags
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Type_PC);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_Type_NPC);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Character_State_BlockHealthRegen);
	}

	namespace BasicAttack
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DamageType_DirectDamage);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Resistance_Modifier_DirectDamage);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Resistance_Ignore_DirectDamage);
	}

	namespace State
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_Health);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_Stamina);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Max_Mana);

		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Blocked_Regen_Health);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Blocked_Regen_Stamina);
	}

	namespace Data
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_Damage);
	}

	namespace Statuses
	{

		// Status Example
		// -- Bleeding --
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Debuff_Bleeding);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DamageType_Bleeding);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Immunity_Attacks);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Immunity_Bleeding);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Immunity_All);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Resistance_Modifier_Bleeding);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Resistance_Ignore_Bleeding);

		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Invisible);	// 투명화 상태
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_Transform);	// 변신
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Status_HonBaek);	// 혼백

		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Debuff_Immobile);	// 정지
	}

	namespace Action
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_LightAttack);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_HeavyAttack);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Dodge);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Sprint);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Jump);

		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Immobilize);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_CloudStep);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_APluckOfMany);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Transformations);
	}

	namespace Cooldown
	{
		// 정지술
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Immobilize);
		// 기화술
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_CloudStep);
		// 분신술
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_APluckOfMany);
		// 변신
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Transformations);
	}

	namespace Input
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_PrimaryAction);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_SecondaryAction);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Dodge);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Sprint);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Jump);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_SkillSlot1);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_SkillSlot2);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_SkillSlot3);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_SkillSlot4);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Transformation);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_DrinkGourd);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Interact);
	}

	namespace Event
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Wukong_SpawnReplication);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Wukong_OpenBox);
	}
}
