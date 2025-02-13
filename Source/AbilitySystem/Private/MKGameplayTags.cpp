// CopyRight KGCA - Team RedCoke


#include "MKGameplayTags.h"

namespace MoKong
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Effect_NoLevel,
								   "Effect.NoLevel",
								   "This GE won't be leveled up or down. Base level will always be the same (upon granting it).")
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Effect_Type_Cooldown, "Effect.Type.Cooldown", "This is a cooldown GE")

	namespace CharacterTags
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Character_Type_PC,
									   "Character.Type.PlayerCharacter",
									   "A Gameplay Tag applied to Characters that are controlled by a local player.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Character_Type_NPC,
									   "Character.Type.NonPlayerCharacter",
									   "A Gameplay Tag applied to Characters that are AI controlled.")
	}

	namespace BasicAttack
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_DamageType_DirectDamage,
									   "DamageType.DirectDamage",
									   "Basic Damage Type. Used to calculated Resistance to Basic Damage.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Resistance_Modifier_DirectDamage,
									   "Resistance.Modifier.DirectDamage",
									   "Granting Resistance to basic Damage")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Resistance_Ignore_DirectDamage,
									   "Resistance.Ignore.DirectDamage",
									   "Ignore Resistance to basic Damage. Will deal true damage.")
	}

	namespace State
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Max_Health, "State.Max.Health", "Max Health reached")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Max_Stamina, "State.Max.Stamina", "Max Stamina reached")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Max_Mana, "State.Max.Mana", "Max Mana reached")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Blocked_Regen_Health,
									   "State.Blocked.Regeneration.Health",
									   "Block Health regeneration.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_State_Blocked_Regen_Stamina,
									   "State.Blocked.Regeneration.Stamina",
									   "Block Stamina regeneration.")
	}

	namespace Data
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Data_Damage, "Data.Damage", "Damage Data")
	}

	namespace Statuses
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Debuff_Bleeding,
									   "Debuff.Bleeding",
									   "Is Bleeding. Can be used to trigger more effects.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_DamageType_Bleeding,
									   "DamageType.Bleeding",
									   "Bleeding Damage Type. Used to calculated Resistance to Bleeding Damage.")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Immunity_Bleeding, "Immunity.Bleeding", "Immune to Bleeding")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Resistance_Modifier_Bleeding,
									   "Resistance.Modifier.Bleeding",
									   "Granting Resistance to Bleeding")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Resistance_Ignore_Bleeding,
									   "Resistance.Ignore.Bleeding",
									   "Ignore Resistance to Bleeding. Will increase bleeding with the source value.")
	}

	namespace Action
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Action_LightAttack, "Action.LightAttack", "경량 공격 중일 때")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Action_HeavyAttack, "Action.HeavyAttack", "중량 공격 중일 때")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Action_Dodge, "Action.Dodge", "회피 중일 때")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Action_Sprint, "Action.Sprint", "달리기 중일 때")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Action_Jump, "Action.Jump", "점프 중일 때")
	}

	namespace Input
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Input_PrimaryAction, "Input.PrimaryAction", "주로 왼쪽 마우스 클릭 (컨트롤러 R2) 등으로 매핑")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Input_SecondaryAction,
									   "Input.SecondaryAction",
									   "주로 오른쪽 마우스 클릭 (컨트롤러 L2) 등으로 매핑")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Input_Dodge, "Input.Dodge", "회피 행동으로 매핑")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Input_Sprint, "Input.Sprint", "달리기 행동으로 매핑")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(TAG_Input_Jump, "Input.Jump", "점프 행동으로 매핑")
	}
}
