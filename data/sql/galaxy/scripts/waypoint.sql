-- --------------------------------------------------------
-- Host: 127.0.0.1
-- Server version: 5.1.63-community - MySQL Community Server (GPL)
-- Server OS: Win64
-- HeidiSQL version: 7.0.0.4053
-- Date/time: 2012-10-14 17:59:01
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.waypoint
DROP TABLE IF EXISTS `waypoint`;
CREATE TABLE IF NOT EXISTS `waypoint` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `owner_id` bigint(20) DEFAULT NULL,
  `x_coordinate` double NOT NULL,
  `y_coordinate` double NOT NULL,
  `z_coordinate` double NOT NULL,
  `is_active` tinyint(1) NOT NULL,
  `planet` varchar(255) NOT NULL,
  `name` varchar(255) NOT NULL,
  `color` smallint(6) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `IDX_B3DC588199E6F5DF` (`owner_id`),
  CONSTRAINT `FK_B3DC588199E6F5DF` FOREIGN KEY (`owner_id`) REFERENCES `object` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=137438953472 DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.waypoint: ~12 rows (approximately)
/*!40000 ALTER TABLE `waypoint` DISABLE KEYS */;
/*!40000 ALTER TABLE `waypoint` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
