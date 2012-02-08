
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

CREATE TABLE IF NOT EXISTS `creature` (
  `id` bigint(20) NOT NULL,
  `owner_id` bigint(20) DEFAULT NULL,
  `musician_id` bigint(20) DEFAULT NULL,
  `bank_credits` bigint(20) unsigned NOT NULL,
  `cash_credits` bigint(20) unsigned NOT NULL,
  `posture` bigint(20) unsigned NOT NULL,
  `faction_rank` smallint(6) unsigned NOT NULL,
  `scale` double NOT NULL,
  `battle_fatigue` bigint(20) NOT NULL,
  `state` bigint(20) NOT NULL,
  `acceleration_base` double NOT NULL,
  `acceleration_modifier` double NOT NULL,
  `speed_base` double NOT NULL,
  `speed_modifier` double NOT NULL,
  `run_speed` double NOT NULL,
  `slope_modifier_angle` double NOT NULL,
  `slope_modifier_percent` double NOT NULL,
  `turn_radius` double NOT NULL,
  `walking_speed` double NOT NULL,
  `water_modifier_percent` double NOT NULL,
  `combat_level` smallint(5) unsigned NOT NULL,
  `animation` varchar(255) NOT NULL,
  `group_id` bigint(20) unsigned DEFAULT NULL,
  `guild_id` int(10) unsigned DEFAULT NULL,
  `weapon_id` bigint(20) unsigned DEFAULT NULL,
  `mood_id` int(11) unsigned NOT NULL,
  `performance_id` int(10) unsigned NOT NULL,
  `disguise_template_id` int(10) unsigned DEFAULT NULL,
  `health_wounds` int(10) unsigned NOT NULL,
  `strength_wounds` int(10) unsigned NOT NULL,
  `constitution_wounds` int(10) unsigned NOT NULL,
  `action_wounds` int(10) unsigned NOT NULL,
  `quickness_wounds` int(10) unsigned NOT NULL,
  `stamina_wounds` int(10) unsigned NOT NULL,
  `mind_wounds` int(10) unsigned NOT NULL,
  `focus_wounds` int(10) unsigned NOT NULL,
  `willpower_wounds` int(10) unsigned NOT NULL,
  `health_encumberance` int(10) unsigned NOT NULL,
  `strength_encumberance` int(10) unsigned NOT NULL,
  `constitution_encumberance` int(10) unsigned NOT NULL,
  `action_encumberance` int(10) unsigned NOT NULL,
  `quickness_encumberance` int(10) unsigned NOT NULL,
  `stamina_encumberance` int(10) unsigned NOT NULL,
  `mind_encumberance` int(10) unsigned NOT NULL,
  `focus_encumberance` int(10) unsigned NOT NULL,
  `willpower_encumberance` int(10) unsigned NOT NULL,
  `current_health` int(10) unsigned NOT NULL,
  `current_strength` int(10) unsigned NOT NULL,
  `current_constitution` int(10) unsigned NOT NULL,
  `current_action` int(10) unsigned NOT NULL,
  `current_quickness` int(10) unsigned NOT NULL,
  `current_stamina` int(10) unsigned NOT NULL,
  `current_mind` int(10) unsigned NOT NULL,
  `current_focus` int(10) unsigned NOT NULL,
  `current_willpower` int(10) unsigned NOT NULL,
  `max_health` int(10) unsigned NOT NULL,
  `max_strength` int(10) unsigned NOT NULL,
  `max_constitution` int(10) unsigned NOT NULL,
  `max_action` int(10) unsigned NOT NULL,
  `max_quickness` int(10) unsigned NOT NULL,
  `max_stamina` int(10) unsigned NOT NULL,
  `max_mind` int(10) unsigned NOT NULL,
  `max_focus` int(10) unsigned NOT NULL,
  `max_willpower` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`),
  KEY `IDX_2A6C6AF47E3C61F9` (`owner_id`),
  KEY `IDX_2A6C6AF49523AA8A` (`musician_id`),
  KEY `FK_2A6C6AF49582386B` (`mood_id`),
  CONSTRAINT `FK_2A6C6AF47E3C61F9` FOREIGN KEY (`owner_id`) REFERENCES `creature` (`id`),
  CONSTRAINT `FK_2A6C6AF49523AA8A` FOREIGN KEY (`musician_id`) REFERENCES `creature` (`id`),
  CONSTRAINT `FK_2A6C6AF49582386B` FOREIGN KEY (`mood_id`) REFERENCES `mood` (`id`),
  CONSTRAINT `FK_A8ADABEC727AC32D` FOREIGN KEY (`disguise_template_id`) REFERENCES `iff_templates` (`id`),
  CONSTRAINT `FK_2A6C6AF4BF396750` FOREIGN KEY (`id`) REFERENCES `object` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DELETE FROM `creature`;
    
/*!40000 ALTER TABLE `creature` DISABLE KEYS */;
INSERT INTO `creature` (`id`, `owner_id`, `musician_id`, `bank_credits`, `cash_credits`, `posture`, `faction_rank`, `scale`, `battle_fatigue`, `state`, `acceleration_base`, `acceleration_modifier`, `speed_base`, `speed_modifier`, `run_speed`, `slope_modifier_angle`, `slope_modifier_percent`, `walking_speed`, `water_modifier_percent`) VALUES
(0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
/*!40000 ALTER TABLE `creature` ENABLE KEYS */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
