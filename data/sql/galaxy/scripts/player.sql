-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:00
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.player
DROP TABLE IF EXISTS `player`;
CREATE TABLE IF NOT EXISTS `player` (
  `id` bigint(20) NOT NULL,
  `profession_tag` varchar(255) NOT NULL,
  `born_date` date NOT NULL,
  `total_playtime` bigint(20) NOT NULL DEFAULT 0,
  `csr_tag` smallint(6) NOT NULL,
  `current_force` int(11) NOT NULL DEFAULT 0,
  `max_force` int(11) NOT NULL DEFAULT 0,
  `experimentation_enabled` tinyint(1) NOT NULL DEFAULT 0,
  `crafting_stage` int(11) NOT NULL DEFAULT 0,
  `nearest_crafting_station` bigint(20) NOT NULL DEFAULT 0,
  `experimentation_points` int(11) NOT NULL DEFAULT 0,
  `accomplishment_counter` int(11) NOT NULL DEFAULT 0,
  `current_language` int(11) NOT NULL DEFAULT 0,
  `current_stomach` int(11) NOT NULL DEFAULT 0,
  `max_stomach` int(11) NOT NULL DEFAULT 0,
  `current_drink` int(11) NOT NULL DEFAULT 0,
  `max_drink` int(11) NOT NULL DEFAULT 0,
  `jedi_state` int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`),
  CONSTRAINT `FK_98197A65BF396750` FOREIGN KEY (`id`) REFERENCES `object` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.player: ~0 rows (approximately)
/*!40000 ALTER TABLE `player` DISABLE KEYS */;
/*!40000 ALTER TABLE `player` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
