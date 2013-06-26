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

-- Dumping structure for table galaxy.xp_list
DROP TABLE IF EXISTS `xp_list`;
CREATE TABLE IF NOT EXISTS `xp_list` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `player_id` bigint(20) DEFAULT NULL,
  `xp_type_id` int(11) DEFAULT NULL,
  `value` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `IDX_960633ED99E6F5DF` (`player_id`),
  KEY `IDX_960633ED2291DC48` (`xp_type_id`),
  CONSTRAINT `FK_960633ED2291DC48` FOREIGN KEY (`xp_type_id`) REFERENCES `xp_type` (`id`),
  CONSTRAINT `FK_960633ED99E6F5DF` FOREIGN KEY (`player_id`) REFERENCES `player` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.xp_list: ~0 rows (approximately)
/*!40000 ALTER TABLE `xp_list` DISABLE KEYS */;
/*!40000 ALTER TABLE `xp_list` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
