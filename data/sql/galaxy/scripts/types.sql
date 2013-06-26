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

-- Dumping structure for table galaxy.types
DROP TABLE IF EXISTS `types`;
CREATE TABLE IF NOT EXISTS `types` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `description` varchar(50) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1874303458 DEFAULT CHARSET=utf8 COMMENT='These are all of the possible object types';

-- Dumping data for table galaxy.types: ~18 rows (approximately)
/*!40000 ALTER TABLE `types` DISABLE KEYS */;
INSERT INTO `types` (`id`, `description`) VALUES
	(0, 'invalid'),
	(1112885583, 'building'),
	(1129465167, 'creature'),
	(1145850183, 'guild'),
	(1178818900, 'factory_crate'),
	(1196578128, 'group'),
	(1229869903, 'installation'),
	(1230261839, 'intangible'),
	(1296651087, 'mission'),
	(1297302351, 'manufacture_schematic'),
	(1347174745, 'player'),
	(1380142671, 'resource_container'),
	(1396919380, 'cell'),
	(1398030671, 'static'),
	(1413566031, 'tangible'),
	(1463900496, 'waypoint'),
	(1789660414, 'weapon'),
	(1874303456, 'ship');
/*!40000 ALTER TABLE `types` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
