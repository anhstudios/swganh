-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:58:57
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.creature
DROP TABLE IF EXISTS `creature`;
CREATE TABLE IF NOT EXISTS `creature` (
  `id` bigint(20) NOT NULL,
  `owner_id` bigint(20) DEFAULT 0,
  `musician_id` bigint(20) DEFAULT 0,
  `bank_credits` bigint(20) unsigned NOT NULL DEFAULT 0,
  `cash_credits` bigint(20) unsigned NOT NULL DEFAULT 0,
  `posture` bigint(20) unsigned NOT NULL DEFAULT 0,
  `faction_rank` smallint(6) unsigned NOT NULL DEFAULT 0,
  `scale` double NOT NULL DEFAULT 0,
  `battle_fatigue` bigint(20) NOT NULL DEFAULT 0,
  `state` bigint(20) NOT NULL DEFAULT 0,
  `acceleration_base` double NOT NULL DEFAULT 0,
  `acceleration_modifier` double NOT NULL DEFAULT 0,
  `speed_base` double NOT NULL DEFAULT 0,
  `speed_modifier` double NOT NULL DEFAULT 0,
  `run_speed` double NOT NULL DEFAULT 0,
  `slope_modifier_angle` double NOT NULL DEFAULT 0,
  `slope_modifier_percent` double NOT NULL DEFAULT 0,
  `turn_radius` double NOT NULL DEFAULT 0,
  `walking_speed` double NOT NULL DEFAULT 0,
  `water_modifier_percent` double NOT NULL DEFAULT 0,
  `combat_level` smallint(5) unsigned NOT NULL DEFAULT 0,
  `animation` varchar(255) NOT NULL DEFAULT '',
  `group_id` bigint(20) unsigned DEFAULT 0,
  `guild_id` int(10) unsigned DEFAULT 0,
  `weapon_id` bigint(20) unsigned DEFAULT 0,
  `mood_id` int(11) unsigned DEFAULT 0,
  `performance_id` int(10) unsigned NOT NULL DEFAULT 0,
  `disguise_template_id` int(10) unsigned DEFAULT 0,
  `health_wounds` int(10) unsigned NOT NULL DEFAULT 0,
  `strength_wounds` int(10) unsigned NOT NULL DEFAULT 0,
  `constitution_wounds` int(10) unsigned NOT NULL DEFAULT 0,
  `action_wounds` int(10) unsigned NOT NULL DEFAULT 0,
  `quickness_wounds` int(10) unsigned NOT NULL DEFAULT 0,
  `stamina_wounds` int(10) unsigned NOT NULL DEFAULT 0,
  `mind_wounds` int(10) unsigned NOT NULL DEFAULT 0,
  `focus_wounds` int(10) unsigned NOT NULL DEFAULT 0,
  `willpower_wounds` int(10) unsigned NOT NULL DEFAULT 0,
  `health_encumberance` int(10) unsigned NOT NULL DEFAULT 0,
  `strength_encumberance` int(10) unsigned NOT NULL DEFAULT 0,
  `constitution_encumberance` int(10) unsigned NOT NULL DEFAULT 0,
  `action_encumberance` int(10) unsigned NOT NULL DEFAULT 0,
  `quickness_encumberance` int(10) unsigned NOT NULL DEFAULT 0,
  `stamina_encumberance` int(10) unsigned NOT NULL DEFAULT 0,
  `mind_encumberance` int(10) unsigned NOT NULL DEFAULT 0,
  `focus_encumberance` int(10) unsigned NOT NULL DEFAULT 0,
  `willpower_encumberance` int(10) unsigned NOT NULL DEFAULT 0,
  `current_health` int(10) unsigned NOT NULL DEFAULT 0,
  `current_strength` int(10) unsigned NOT NULL DEFAULT 0,
  `current_constitution` int(10) unsigned NOT NULL DEFAULT 0,
  `current_action` int(10) unsigned NOT NULL DEFAULT 0,
  `current_quickness` int(10) unsigned NOT NULL DEFAULT 0,
  `current_stamina` int(10) unsigned NOT NULL DEFAULT 0,
  `current_mind` int(10) unsigned NOT NULL DEFAULT 0,
  `current_focus` int(10) unsigned NOT NULL DEFAULT 0,
  `current_willpower` int(10) unsigned NOT NULL DEFAULT 0,
  `max_health` int(10) unsigned NOT NULL DEFAULT 0,
  `max_strength` int(10) unsigned NOT NULL DEFAULT 0,
  `max_constitution` int(10) unsigned NOT NULL DEFAULT 0,
  `max_action` int(10) unsigned NOT NULL DEFAULT 0,
  `max_quickness` int(10) unsigned NOT NULL DEFAULT 0,
  `max_stamina` int(10) unsigned NOT NULL DEFAULT 0,
  `max_mind` int(10) unsigned NOT NULL DEFAULT 0,
  `max_focus` int(10) unsigned NOT NULL DEFAULT 0,
  `max_willpower` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  KEY `IDX_2A6C6AF47E3C61F9` (`owner_id`),
  KEY `IDX_2A6C6AF49523AA8A` (`musician_id`),
  KEY `FK_A8ADABEC727AC32D` (`disguise_template_id`),
  CONSTRAINT `FK_812371D251G23` FOREIGN KEY (`id`) REFERENCES `object` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40000 ALTER TABLE `creature` DISABLE KEYS */;
INSERT INTO `creature` (`id`, `owner_id`, `musician_id`, `bank_credits`, `cash_credits`, `posture`, `faction_rank`, `scale`, `battle_fatigue`, `state`, `acceleration_base`, `acceleration_modifier`, `speed_base`, `speed_modifier`, `run_speed`, `slope_modifier_angle`, `slope_modifier_percent`, `turn_radius`, `walking_speed`, `water_modifier_percent`, `combat_level`, `animation`, `group_id`, `guild_id`, `weapon_id`, `mood_id`, `performance_id`, `disguise_template_id`, `health_wounds`, `strength_wounds`, `constitution_wounds`, `action_wounds`, `quickness_wounds`, `stamina_wounds`, `mind_wounds`, `focus_wounds`, `willpower_wounds`, `health_encumberance`, `strength_encumberance`, `constitution_encumberance`, `action_encumberance`, `quickness_encumberance`, `stamina_encumberance`, `mind_encumberance`, `focus_encumberance`, `willpower_encumberance`, `current_health`, `current_strength`, `current_constitution`, `current_action`, `current_quickness`, `current_stamina`, `current_mind`, `current_focus`, `current_willpower`, `max_health`, `max_strength`, `max_constitution`, `max_action`, `max_quickness`, `max_stamina`, `max_mind`, `max_focus`, `max_willpower`) VALUES
	(0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', NULL, NULL, NULL, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
	;
/*!40000 ALTER TABLE `creature` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
