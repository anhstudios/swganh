# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.2-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-26 22:53:03
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy.creature
CREATE TABLE `creature` (
	`id` BIGINT(20) NOT NULL,
	`owner_id` BIGINT(20) NULL DEFAULT NULL,
	`musician_id` BIGINT(20) NULL DEFAULT NULL,
	`bank_credits` BIGINT(20) UNSIGNED NOT NULL,
	`cash_credits` BIGINT(20) UNSIGNED NOT NULL,
	`posture` BIGINT(20) UNSIGNED NOT NULL,
	`faction_rank` SMALLINT(6) UNSIGNED NOT NULL,
	`scale` DOUBLE NOT NULL,
	`battle_fatigue` BIGINT(20) NOT NULL,
	`state` BIGINT(20) NOT NULL,
	`acceleration_base` DOUBLE NOT NULL,
	`acceleration_modifier` DOUBLE NOT NULL,
	`speed_base` DOUBLE NOT NULL,
	`speed_modifier` DOUBLE NOT NULL,
	`run_speed` DOUBLE NOT NULL,
	`slope_modifier_angle` DOUBLE NOT NULL,
	`slope_modifier_percent` DOUBLE NOT NULL,
	`turn_radius` DOUBLE NOT NULL,
	`walking_speed` DOUBLE NOT NULL,
	`water_modifier_percent` DOUBLE NOT NULL,
	`combat_level` SMALLINT(5) UNSIGNED NOT NULL,
	`animation` VARCHAR(255) NOT NULL,
	`group_id` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
	`guild_id` INT(10) UNSIGNED NULL DEFAULT NULL,
	`weapon_id` BIGINT(20) UNSIGNED NULL DEFAULT NULL,
	`mood_id` INT(11) UNSIGNED NOT NULL DEFAULT 0,
	`performance_id` INT(10) UNSIGNED NOT NULL,
	`disguise_template_id` INT(10) NULL DEFAULT NULL,
	`health_wounds` INT(10) UNSIGNED NOT NULL,
	`strength_wounds` INT(10) UNSIGNED NOT NULL,
	`constitution_wounds` INT(10) UNSIGNED NOT NULL,
	`action_wounds` INT(10) UNSIGNED NOT NULL,
	`quickness_wounds` INT(10) UNSIGNED NOT NULL,
	`stamina_wounds` INT(10) UNSIGNED NOT NULL,
	`mind_wounds` INT(10) UNSIGNED NOT NULL,
	`focus_wounds` INT(10) UNSIGNED NOT NULL,
	`willpower_wounds` INT(10) UNSIGNED NOT NULL,
	`health_encumberance` INT(10) UNSIGNED NOT NULL,
	`strength_enumberance` INT(10) UNSIGNED NOT NULL,
	`constitution_encumberance` INT(10) UNSIGNED NOT NULL,
	`action_encumberance` INT(10) UNSIGNED NOT NULL,
	`quickness_encumberance` INT(10) UNSIGNED NOT NULL,
	`stamina_encumberance` INT(10) UNSIGNED NOT NULL,
	`mind_encumberance` INT(10) UNSIGNED NOT NULL,
	`focus_encumberance` INT(10) UNSIGNED NOT NULL,
	`willpower_encumberance` INT(10) UNSIGNED NOT NULL,
	`current_health` INT(10) UNSIGNED NOT NULL,
	`current_strength` INT(10) UNSIGNED NOT NULL,
	`current_constitution` INT(10) UNSIGNED NOT NULL,
	`current_action` INT(10) UNSIGNED NOT NULL,
	`current_quickness` INT(10) UNSIGNED NOT NULL,
	`current_stamina` INT(10) UNSIGNED NOT NULL,
	`current_mind` INT(10) UNSIGNED NOT NULL,
	`current_focus` INT(10) UNSIGNED NOT NULL,
	`current_willpower` INT(10) UNSIGNED NOT NULL,
	`max_health` INT(10) UNSIGNED NOT NULL,
	`max_strength` INT(10) UNSIGNED NOT NULL,
	`max_constitution` INT(10) UNSIGNED NOT NULL,
	`max_action` INT(10) UNSIGNED NOT NULL,
	`max_quickness` INT(10) UNSIGNED NOT NULL,
	`max_stamina` INT(10) UNSIGNED NOT NULL,
	`max_mind` INT(10) UNSIGNED NOT NULL,
	`max_focus` INT(10) UNSIGNED NOT NULL,
	`max_willpower` INT(10) UNSIGNED NOT NULL,
	PRIMARY KEY (`id`),
	INDEX `IDX_2A6C6AF47E3C61F9` (`owner_id`),
	INDEX `IDX_2A6C6AF49523AA8A` (`musician_id`),
	CONSTRAINT `FK_2A6C6AF47E3C61F9` FOREIGN KEY (`owner_id`) REFERENCES `creature` (`id`),
	CONSTRAINT `FK_2A6C6AF49523AA8A` FOREIGN KEY (`musician_id`) REFERENCES `creature` (`id`),
	CONSTRAINT `FK_2A6C6AF49582386B` FOREIGN KEY (`mood_id`) REFERENCES `mood` (`id`),
	CONSTRAINT `FK_2A6C6AF4BF396750` FOREIGN KEY (`id`) REFERENCES `object` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.creature: ~9 rows (approximately)
DELETE FROM `creature`;
/*!40000 ALTER TABLE `creature` DISABLE KEYS */;
INSERT INTO `creature` (`id`, `owner_id`, `musician_id`, `bank_credits`, `cash_credits`, `posture`, `faction_rank`, `scale`, `battle_fatigue`, `state`, `acceleration_base`, `acceleration_modifier`, `speed_base`, `speed_modifier`, `run_speed`, `slope_modifier_angle`, `slope_modifier_percent`, `walking_speed`, `water_modifier_percent`) VALUES
	(0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
