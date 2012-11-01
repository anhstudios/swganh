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

-- Dumping structure for table galaxy.players_status_flags
DROP TABLE IF EXISTS `players_status_flags`;
CREATE TABLE IF NOT EXISTS `players_status_flags` (
  `player_id` bigint(20) NOT NULL,
  `status_flag_id` int(11) NOT NULL,
  PRIMARY KEY (`player_id`,`status_flag_id`),
  KEY `IDX_9475E7A299E6F5DF` (`player_id`),
  KEY `IDX_9475E7A2994F9360` (`status_flag_id`),
  CONSTRAINT `FK_9475E7A2994F9360` FOREIGN KEY (`status_flag_id`) REFERENCES `status_flag` (`id`),
  CONSTRAINT `FK_9475E7A299E6F5DF` FOREIGN KEY (`player_id`) REFERENCES `player` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.players_status_flags: ~0 rows (approximately)
/*!40000 ALTER TABLE `players_status_flags` DISABLE KEYS */;
/*!40000 ALTER TABLE `players_status_flags` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
