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

-- Dumping structure for table galaxy.players_profile_flags
DROP TABLE IF EXISTS `players_profile_flags`;
CREATE TABLE IF NOT EXISTS `players_profile_flags` (
  `player_id` bigint(20) NOT NULL,
  `profile_flag_id` int(11) NOT NULL,
  PRIMARY KEY (`player_id`,`profile_flag_id`),
  KEY `IDX_52689ED599E6F5DF` (`player_id`),
  KEY `IDX_52689ED542213A79` (`profile_flag_id`),
  CONSTRAINT `FK_52689ED542213A79` FOREIGN KEY (`profile_flag_id`) REFERENCES `profile_flag` (`id`),
  CONSTRAINT `FK_52689ED599E6F5DF` FOREIGN KEY (`player_id`) REFERENCES `player` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.players_profile_flags: ~0 rows (approximately)
/*!40000 ALTER TABLE `players_profile_flags` DISABLE KEYS */;
/*!40000 ALTER TABLE `players_profile_flags` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
