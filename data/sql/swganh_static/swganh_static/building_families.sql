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

-- Dumping structure for table swganh_static.building_families
DROP TABLE IF EXISTS `building_families`;
CREATE TABLE IF NOT EXISTS `building_families` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Building Family ID',
  `family_name` varchar(255) NOT NULL DEFAULT '""' COMMENT 'Building Family Name',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.building_families: ~13 rows (approximately)
/*!40000 ALTER TABLE `building_families` DISABLE KEYS */;
INSERT INTO `building_families` (`id`, `family_name`) VALUES
	(0, 'none'),
	(1, 'parking garage'),
	(2, 'cantina'),
	(3, 'guildhall'),
	(4, 'hotel'),
	(5, 'medicalcenter'),
	(6, 'shuttleport'),
	(7, 'starport'),
	(8, 'themepark'),
	(9, 'theater'),
	(10, 'salon'),
	(11, 'cloning facility'),
	(12, 'player structure');
/*!40000 ALTER TABLE `building_families` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
