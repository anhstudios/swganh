-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:01
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.waypoint
DROP TABLE IF EXISTS `waypoint`;
CREATE TABLE IF NOT EXISTS `waypoint` (
  `id` bigint(20) NOT NULL,
  `player_id` bigint(20) DEFAULT NULL,
  `is_active` tinyint(1) NOT NULL,
  `name` varchar(255) NOT NULL,
  `color` smallint(6) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `IDX_B3DC588199E6F5DF` (`player_id`),
  CONSTRAINT `FK_B3DC588199E6F5DF` FOREIGN KEY (`player_id`) REFERENCES `player` (`id`),
  CONSTRAINT `FK_B3DC5881BF396750` FOREIGN KEY (`id`) REFERENCES `object` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.waypoint: ~12 rows (approximately)
/*!40000 ALTER TABLE `waypoint` DISABLE KEYS */;
INSERT INTO `waypoint` (`id`, `player_id`, `is_active`, `name`, `color`) VALUES
	(15497, NULL, 1, '', 0),
	(15498, NULL, 1, '', 0),
	(15499, NULL, 1, '', 0),
	(15500, NULL, 1, '', 0),
	(15501, NULL, 1, '', 0),
	(15502, NULL, 1, '', 0),
	(15503, NULL, 1, '', 0),
	(15504, NULL, 1, '', 0),
	(15505, NULL, 1, '', 0),
	(15506, NULL, 1, '', 0),
	(15507, NULL, 1, '', 0),
	(15508, NULL, 1, '', 0);
/*!40000 ALTER TABLE `waypoint` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
