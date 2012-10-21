-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:20
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.travel_planet_routes
DROP TABLE IF EXISTS `travel_planet_routes`;
CREATE TABLE IF NOT EXISTS `travel_planet_routes` (
  `srcId` int(11) NOT NULL COMMENT 'Source Planet ID',
  `destId` int(11) NOT NULL DEFAULT '0' COMMENT 'Destination Planet ID',
  `price` int(11) unsigned NOT NULL DEFAULT '1000' COMMENT 'Travel Route Price',
  PRIMARY KEY (`srcId`,`destId`),
  KEY `travel_route_destination_planet_id` (`destId`),
  CONSTRAINT `travel_route_destination_planet_id` FOREIGN KEY (`destId`) REFERENCES `planet` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `travel_route_source_planet_id` FOREIGN KEY (`srcId`) REFERENCES `planet` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.travel_planet_routes: ~37 rows (approximately)
/*!40000 ALTER TABLE `travel_planet_routes` DISABLE KEYS */;
INSERT INTO `travel_planet_routes` (`srcId`, `destId`, `price`) VALUES
	(0, 0, 100),
	(0, 1, 1000),
	(0, 2, 2000),
	(0, 3, 4000),
	(0, 5, 500),
	(0, 7, 300),
	(0, 8, 600),
	(0, 9, 3000),
	(0, 42, 1250),
	(1, 0, 1000),
	(1, 1, 100),
	(2, 0, 2000),
	(2, 2, 100),
	(3, 0, 4000),
	(3, 3, 100),
	(3, 5, 1750),
	(4, 4, 100),
	(4, 5, 1250),
	(4, 8, 1250),
	(5, 0, 500),
	(5, 3, 1750),
	(5, 4, 1250),
	(5, 5, 100),
	(5, 6, 300),
	(5, 8, 500),
	(6, 5, 300),
	(6, 6, 100),
	(7, 0, 300),
	(7, 7, 100),
	(8, 0, 600),
	(8, 4, 1250),
	(8, 5, 500),
	(8, 8, 100),
	(9, 0, 3000),
	(9, 9, 100),
	(42, 0, 1250),
	(42, 42, 100);
/*!40000 ALTER TABLE `travel_planet_routes` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
