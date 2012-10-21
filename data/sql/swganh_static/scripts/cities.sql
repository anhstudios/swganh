-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:15
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.cities
DROP TABLE IF EXISTS `cities`;
CREATE TABLE IF NOT EXISTS `cities` (
  `id` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT 'City ID',
  `city_name` varchar(255) NOT NULL DEFAULT 'city_name' COMMENT 'City Name',
  `city_region` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'City Region',
  `planet_id` int(11) NOT NULL DEFAULT '0' COMMENT 'City Planet',
  PRIMARY KEY (`id`),
  UNIQUE KEY `city_name` (`city_name`,`planet_id`),
  KEY `fk_cities_planet_planet` (`planet_id`),
  KEY `fk_cities_region_region` (`city_region`),
  CONSTRAINT `city_planet_id` FOREIGN KEY (`planet_id`) REFERENCES `planet` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.cities: ~36 rows (approximately)
/*!40000 ALTER TABLE `cities` DISABLE KEYS */;
INSERT INTO `cities` (`id`, `city_name`, `city_region`, `planet_id`) VALUES
	(64424509440, 'Coronet', 4, 0),
	(64424509441, 'Tyrena', 5, 0),
	(64424509442, 'Bela Vista', 6, 0),
	(64424509443, 'Kor Vela', 7, 0),
	(64424509444, 'Doaba Guerfel', 8, 0),
	(64424509445, 'Vreni Island', 9, 0),
	(64424509446, 'Mining Outpost', 1, 1),
	(64424509447, 'Pirate Outpost', 2, 1),
	(64424509448, 'Imperial Outpost', 3, 1),
	(64424509449, 'Trade Outpost', 10, 2),
	(64424509450, 'Science Outpost', 11, 2),
	(64424509451, 'Research Outpost', 12, 3),
	(64424509452, 'Smuggler\'s Outpost', 13, 3),
	(64424509453, 'Nym\'s Stronghold', 14, 4),
	(64424509454, 'Imperial Outpost', 15, 4),
	(64424509455, 'Theed', 16, 5),
	(64424509456, 'Keren', 17, 5),
	(64424509457, 'Moenia', 18, 5),
	(64424509458, 'Deeja Peek', 19, 5),
	(64424509459, 'Kaadara', 20, 5),
	(64424509460, 'Lake Retreat', 37, 5),
	(64424509461, 'Narmle', 21, 6),
	(64424509462, 'Restuss', 22, 6),
	(64424509463, 'Rebel Outpost', 23, 6),
	(64424509464, 'Dearic', 24, 7),
	(64424509465, 'Nashal', 25, 7),
	(64424509466, 'Imperial Outpost', 26, 7),
	(64424509467, 'Bestine', 27, 8),
	(64424509468, 'Mos Espa', 28, 8),
	(64424509469, 'Mos Eisley', 29, 8),
	(64424509470, 'Mos Entha', 30, 8),
	(64424509471, 'Mos Taike', 31, 8),
	(64424509472, 'Anchorhead', 32, 8),
	(64424509473, 'Wayfar', 33, 8),
	(64424509474, 'Labor Outpost', 34, 9),
	(64424509475, 'Mining Outpost', 35, 9);
/*!40000 ALTER TABLE `cities` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
