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
  `total_playtime` bigint(20) NOT NULL,
  `csr_tag` smallint(6) NOT NULL,
  `current_force` int(11) NOT NULL,
  `max_force` int(11) NOT NULL,
  `experimentation_enabled` tinyint(1) NOT NULL,
  `crafting_stage` int(11) NOT NULL,
  `nearest_crafting_station` bigint(20) NOT NULL,
  `experimentation_points` int(11) NOT NULL,
  `accomplishment_counter` int(11) NOT NULL,
  `current_language` int(11) NOT NULL,
  `current_stomach` int(11) NOT NULL,
  `max_stomach` int(11) NOT NULL,
  `current_drink` int(11) NOT NULL,
  `max_drink` int(11) NOT NULL,
  `jedi_state` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `FK_98197A65BF396750` FOREIGN KEY (`id`) REFERENCES `object` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table galaxy.player: ~0 rows (approximately)
/*!40000 ALTER TABLE `player` DISABLE KEYS */;
/*!40000 ALTER TABLE `player` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
