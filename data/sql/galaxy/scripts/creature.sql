# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-15 23:25:17
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
  PRIMARY KEY (`id`),
  KEY `IDX_2A6C6AF47E3C61F9` (`owner_id`),
  KEY `IDX_2A6C6AF49523AA8A` (`musician_id`),
  CONSTRAINT `FK_2A6C6AF47E3C61F9` FOREIGN KEY (`owner_id`) REFERENCES `creature` (`id`),
  CONSTRAINT `FK_2A6C6AF49523AA8A` FOREIGN KEY (`musician_id`) REFERENCES `creature` (`id`),
  CONSTRAINT `FK_2A6C6AF4BF396750` FOREIGN KEY (`id`) REFERENCES `object` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.creature: ~9 rows (approximately)
/*!40000 ALTER TABLE `creature` DISABLE KEYS */;
INSERT INTO `creature` (`id`, `owner_id`, `musician_id`, `bank_credits`, `cash_credits`, `posture`, `faction_rank`, `scale`, `battle_fatigue`, `state`, `acceleration_base`, `acceleration_modifier`, `speed_base`, `speed_modifier`, `run_speed`, `slope_modifier_angle`, `slope_modifier_percent`, `walking_speed`, `water_modifier_percent`) VALUES
	(0, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(8589934598, 0, NULL, 2000, 0, 0, 0, 0.907768249511719, 0, 0, 1, 5, 1, 5, 10, 5, 5, 5, 0),
	(8589934603, 0, NULL, 2000, 0, 0, 0, 1.0537543296814, 0, 0, 1, 5, 1, 5, 10, 5, 5, 5, 0),
	(8589934613, 0, NULL, 2000, 0, 0, 0, 1.00736689567566, 0, 0, 1, 5, 1, 5, 10, 5, 5, 5, 0),
	(8589934623, 0, NULL, 2000, 0, 0, 0, 1.00736689567566, 0, 0, 1, 5, 1, 5, 10, 5, 5, 5, 0),
	(8589934633, 0, NULL, 2000, 0, 0, 0, 1.00736689567566, 0, 0, 1, 5, 1, 5, 10, 5, 5, 5, 0),
	(8589934643, 0, NULL, 2000, 0, 0, 0, 1.00736689567566, 0, 0, 1, 5, 1, 5, 10, 5, 5, 5, 0),
	(8589934659, 0, NULL, 2000, 0, 0, 0, 0.928309619426727, 0, 0, 1, 5, 1, 5, 10, 5, 5, 5, 0),
	(8589934675, 0, NULL, 2000, 0, 0, 0, 0.996987223625183, 0, 0, 1, 5, 1, 5, 10, 5, 5, 5, 0),
	(8589934691, 0, NULL, 2000, 0, 0, 0, 1.1090430021286, 0, 0, 1, 5, 1, 5, 10, 5, 5, 5, 0);
/*!40000 ALTER TABLE `creature` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
