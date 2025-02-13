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
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Immunity_Bleeding);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Resistance_Modifier_Bleeding);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Resistance_Ignore_Bleeding);
	}

	namespace Action
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_LightAttack);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_HeavyAttack);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Dodge);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Sprint);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Action_Jump);
	}

	namespace Input
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_PrimaryAction);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_SecondaryAction);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Dodge);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Sprint);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Jump);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_)
	}
}
