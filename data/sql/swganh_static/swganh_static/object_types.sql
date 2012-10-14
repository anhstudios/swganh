-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:18
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.object_types
DROP TABLE IF EXISTS `object_types`;
CREATE TABLE IF NOT EXISTS `object_types` (
  `id` int(10) unsigned NOT NULL,
  `object_type` bigint(20) unsigned NOT NULL DEFAULT '0',
  `description` varchar(128) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.object_types: ~18 rows (approximately)
/*!40000 ALTER TABLE `object_types` DISABLE KEYS */;
INSERT INTO `object_types` (`id`, `object_type`, `description`) VALUES
	(1, 1112885583, 'building'),
	(2, 1129465167, 'creature'),
	(3, 1145850183, 'guild'),
	(4, 1178818900, 'factory_crate'),
	(5, 1196578128, 'group'),
	(6, 1229869903, 'installation'),
	(7, 1230261839, 'intangible'),
	(8, 1296651087, 'mission'),
	(9, 1297302351, 'manufacture_schematic'),
	(10, 1347174745, 'player'),
	(11, 1380142671, 'resource_container'),
	(12, 1396919380, 'cell'),
	(13, 1398030671, 'static'),
	(14, 1413566031, 'tangible'),
	(15, 1463900496, 'waypoint'),
	(16, 1789660414, 'weapon'),
	(17, 1874303456, 'ship'),
	(18, 0, 'invalid');
/*!40000 ALTER TABLE `object_types` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
