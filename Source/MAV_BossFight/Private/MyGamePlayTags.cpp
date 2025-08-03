// Vince Petrelli All Rights Reserved


#include "MyGamePlayTags.h"

namespace MyGameplayTags
{
	/** Input Tags **/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move,"InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look,"InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_EquipAxe,"InputTag.EquipAxe");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UnequipAxe,"InputTag.UnEquipAxe");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_Axe,"InputTag.LightAttack.Axe");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_HeavyAttack_Axe,"InputTag.HeavyAttack.Axe");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHeld,"InputTag.MustBeHeld");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHeld_Block,"InputTag.MustBeHeld.Block");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHeld_Run,"InputTag.MustBeHeld.Run");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Roll,"InputTag.Roll");

	/** Player Tags **/

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Axe,"Player.Ability.Equip.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Axe,"Player.Ability.Unequip.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_LightAttack_Axe,"Player.Ability.Attack.Light.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HeavyAttack_Axe,"Player.Ability.Attack.Heavy.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Block,"Player.Ability.Block");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Run,"Player.Ability.Run");

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Roll,"Player.Ability.Roll");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_BlockRegen,"Player.Status.BlockRegen");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Rolling,"Player.Status.Rolling");



	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Axe,"Player.Weapon.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Axe,"Player.Event.Equip.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Axe,"Player.Event.Unequip.Axe");
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Light,"Player.SetByCaller.AttackType.Light");

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Blocked,"Player.Event.Blocked");


	/** Enemy tags **/

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon,"Enemy.Weapon");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee,"Enemy.Ability.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Ranged,"Enemy.Ability.Ranged");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Blocking,"Player.Status.Blocking");

	/** Shared tags **/
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact,"Shared.Ability.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit,"Shared.Event.MeleeHit");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact,"Shared.Event.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseDamage,"Shared.SetByCaller.BaseDamage");

	
}