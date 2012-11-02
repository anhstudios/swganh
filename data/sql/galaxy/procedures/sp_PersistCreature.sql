/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for procedure galaxy.sp_PersistCreature
DROP PROCEDURE IF EXISTS `sp_PersistCreature`;
DELIMITER //
CREATE PROCEDURE `sp_PersistCreature`(
    IN `object_id` BIGINT, 
    IN `in_owner_id` BIGINT, 
    IN `in_musician_id` BIGINT, 
    IN `in_bank_credits` BIGINT, 
    IN `in_cash_credits` BIGINT, 
    IN `in_posture` BIGINT, 
    IN `in_faction_rank` SMALLINT, 
    IN `in_scale` DOUBLE, 
    IN `in_battle_fatigue` BIGINT, 
    IN `in_state` BIGINT, 
    IN `in_acceleration_base` DOUBLE, 
    IN `in_acceleration_modifier` DOUBLE, 
    IN `in_speed_base` DOUBLE, 
    IN `in_speed_modifier` DOUBLE, 
    IN `in_run_speed` DOUBLE, 
    IN `in_slope_modifier_angle` DOUBLE, 
    IN `in_slope_modifier_percent` DOUBLE, 
    IN `in_turn_radius` DOUBLE, 
    IN `in_walking_speed` DOUBLE, 
    IN `in_water_modifier_percent` DOUBLE, 
    IN `in_combat_level` SMALLINT, 
    IN `in_animation` VARCHAR(255), 
    IN `in_group_id` BIGINT, 
    IN `in_guild_id` INT, 
    IN `in_weapon_id` BIGINT, 
    IN `in_mood_id` TINYINT, 
    IN `in_performance_id` INT, 
    IN `in_disguise` VARCHAR(255), 
    IN `in_health_wounds` INT, 
    IN `in_strength_wounds` INT, 
    IN `in_constitution_wounds` INT, 
    IN `in_action_wounds` INT, 
    IN `in_quickness_wounds` INT, 
    IN `in_stamina_wounds` INT, 
    IN `in_mind_wounds` INT, 
    IN `in_focus_wounds` INT, 
    IN `in_willpower_wounds` INT, 
    IN `in_health_encumberance` INT, 
    IN `in_strength_encumberance` INT, 
    IN `in_constitution_encumberance` INT, 
    IN `in_action_encumberance` INT, 
    IN `in_quickness_encumberance` INT, 
    IN `in_stamina_encumberance` INT, 
    IN `in_mind_encumberance` INT, 
    IN `in_focus_encumberance` INT, 
    IN `in_willpower_encumberance` INT, 
    IN `in_current_health` INT, 
    IN `in_current_strength` INT, 
    IN `in_current_constitution` INT, 
    IN `in_current_action` INT, 
    IN `in_current_quickness` INT, 
    IN `in_current_stamina` INT, 
    IN `in_current_mind` INT, 
    IN `in_current_focus` INT, 
    IN `in_current_willpower` INT, 
    IN `in_max_health` INT, 
    IN `in_max_strength` INT, 
    IN `in_max_constitution` INT, 
    IN `in_max_action` INT, 
    IN `in_max_quickness` INT, 
    IN `in_max_stamina` INT, 
    IN `in_max_mind` INT, 
    IN `in_max_focus` INT, 
    IN `in_max_willpower` INT)
BEGIN
-- Get Disguise Template
    DECLARE disguise_iff_template INT;
	DECLARE does_exist INT;
    
    select count(*) from creature c where c.id = object_id into does_exist;
    SELECT swganh_static.iff_templates.id FROM swganh_static.iff_templates WHERE swganh_static.iff_templates.iff_template LIKE in_disguise INTO disguise_iff_template;
--
	IF does_exist > 0 THEN
			update creature set owner_id = in_owner_id, musician_id = in_musician_id, bank_credits = in_bank_credits, cash_credits = in_cash_credits,
				posture = in_posture, faction_rank = in_faction_rank, scale = in_scale, battle_fatigue = in_battle_fatigue, `state` = in_state,
				acceleration_base = in_acceleration_base, acceleration_modifier = in_acceleration_modifier, speed_base = in_speed_base, speed_modifier = in_speed_modifier,
				run_speed = in_run_speed, slope_modifier_angle = in_slope_modifier_angle, slope_modifier_percent = in_slope_modifier_percent, turn_radius = in_turn_radius,
				walking_speed = in_walking_speed, water_modifier_percent = in_water_modifier_percent, combat_level = in_combat_level, animation = in_animation,
				group_id = in_group_id, guild_id = in_guild_id, weapon_id = in_weapon_id, mood_id = in_mood_id,
				performance_id = in_performance_id, disguise_template_id = disguise_iff_template, health_wounds = in_health_wounds, strength_wounds = in_strength_wounds,
				constitution_wounds = in_constitution_wounds, action_wounds = in_action_wounds, quickness_wounds = in_quickness_wounds, stamina_wounds = in_stamina_wounds,
				mind_wounds = in_mind_wounds, focus_wounds = in_focus_wounds, willpower_wounds = in_willpower_wounds, health_encumberance = in_health_encumberance, strength_encumberance = in_strength_encumberance,
				constitution_encumberance = in_constitution_encumberance, action_encumberance = in_action_encumberance, quickness_encumberance = in_quickness_encumberance, stamina_encumberance = in_stamina_encumberance,
				mind_encumberance = in_mind_encumberance, focus_encumberance = in_focus_encumberance, willpower_encumberance = in_willpower_encumberance,
				current_health = in_current_health, current_strength = in_current_strength,current_constitution = in_current_constitution,
				current_action = in_current_action, current_quickness = in_current_quickness, current_stamina = in_current_stamina,
				current_mind = in_current_mind, current_focus = in_current_focus, current_willpower = in_current_willpower,
				max_health = in_max_health, max_strength = in_max_strength,max_constitution = in_max_constitution,
				max_action = in_max_action, max_quickness = in_max_quickness, max_stamina = in_max_stamina,
				max_mind = in_max_mind, max_focus = in_max_focus, max_willpower = in_max_willpower
		where creature.id = object_id;
	ELSE
		insert into creature (id, owner_id, musician_id, bank_credits, cash_credits,
				posture, faction_rank, scale, battle_fatigue, `state`,
				acceleration_base, acceleration_modifier, speed_base, speed_modifier,
				run_speed, slope_modifier_angle, slope_modifier_percent, turn_radius,
				walking_speed, water_modifier_percent, combat_level, animation,
				group_id, guild_id, weapon_id, mood_id,
				performance_id, disguise_template_id, health_wounds, strength_wounds,
				constitution_wounds, action_wounds, quickness_wounds, stamina_wounds,
				mind_wounds, focus_wounds, willpower_wounds, health_encumberance, strength_encumberance,
				constitution_encumberance, action_encumberance, quickness_encumberance, stamina_encumberance,
				mind_encumberance, focus_encumberance, willpower_encumberance,
				current_health, current_strength,current_constitution,
				current_action, current_quickness, current_stamina,
				current_mind, current_focus, current_willpower,
				max_health, max_strength,max_constitution,
				max_action, max_quickness, max_stamina,
				max_mind, max_focus, max_willpower)
			values (object_id, in_owner_id, in_musician_id, in_bank_credits, in_cash_credits, in_posture, in_faction_rank, in_scale, in_battle_fatigue, in_state, in_acceleration_base, 	
				in_acceleration_modifier, in_speed_base, in_speed_modifier, in_run_speed, in_slope_modifier_angle, in_slope_modifier_percent, 
				in_turn_radius, in_walking_speed, in_water_modifier_percent, in_combat_level,in_animation, in_group_id, in_guild_id, in_weapon_id, in_mood_id, in_performance_id, disguise_iff_template, in_health_wounds, in_strength_wounds, in_constitution_wounds,
				in_action_wounds, in_quickness_wounds, in_stamina_wounds, in_mind_wounds, in_focus_wounds, in_willpower_wounds, in_health_encumberance, in_strength_encumberance, in_constitution_encumberance, 
				in_action_encumberance, in_quickness_encumberance, in_stamina_encumberance, in_mind_encumberance, in_focus_encumberance, in_willpower_encumberance, in_current_health, in_current_strength, in_current_constitution,
				in_current_action, in_current_quickness, in_current_stamina, in_current_mind, in_current_focus, in_current_willpower, in_max_health, in_max_strength, in_max_constitution, in_max_action, in_max_quickness,
				in_max_stamina, in_max_mind, in_max_focus, in_max_willpower)
		;
	END IF;
END//
DELIMITER ;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
