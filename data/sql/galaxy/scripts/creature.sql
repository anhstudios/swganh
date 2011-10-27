# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.2-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-26 22:15:44
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy.creature
CREATE TABLE IF NOT EXISTS `creature` (
  `id` bigint(20) NOT NULL,
  `owner_id` bigint(20) DEFAULT NULL,
  `musician_id` bigint(20) DEFAULT NULL,
  `bank_credits` bigint(20) NOT NULL,
  `cash_credits` bigint(20) NOT NULL,
  `posture` bigint(20) NOT NULL,
  `faction_rank` smallint(6) NOT NULL,
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
  `walking_speed` double NOT NULL,
  `water_modifier_percent` double NOT NULL,
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
  `strength_enumberance` int(10) unsigned NOT NULL,
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
  CONSTRAINT `FK_2A6C6AF47E3C61F9` FOREIGN KEY (`owner_id`) REFERENCES `creature` (`id`),
  CONSTRAINT `FK_2A6C6AF49523AA8A` FOREIGN KEY (`musician_id`) REFERENCES `creature` (`id`),
  CONSTRAINT `FK_2A6C6AF4BF396750` FOREIGN KEY (`id`) REFERENCES `object` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Data exporting was unselected.
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
