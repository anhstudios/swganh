-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:17
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.holoemote
DROP TABLE IF EXISTS `holoemote`;
CREATE TABLE IF NOT EXISTS `holoemote` (
  `id` int(10) NOT NULL DEFAULT '0' COMMENT 'Holoemote ID',
  `crc` int(10) unsigned NOT NULL COMMENT 'Holoemote CRC',
  `effect_id` int(10) unsigned NOT NULL COMMENT 'Holoemote Effect ID',
  `name` varchar(45) NOT NULL COMMENT 'Holoemote Name',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Entertainer - HoloEmotes';

-- Dumping data for table swganh_static.holoemote: ~16 rows (approximately)
/*!40000 ALTER TABLE `holoemote` DISABLE KEYS */;
INSERT INTO `holoemote` (`id`, `crc`, `effect_id`, `name`) VALUES
	(1, 0, 0, 'all'),
	(2, 271843963, 666, 'champagne'),
	(3, 1016410324, 673, 'beehive'),
	(4, 1031957463, 674, 'rebel'),
	(5, 1689610579, 680, 'imperial'),
	(6, 1855276851, 683, 'blossom'),
	(7, 2206631595, 684, 'hearts'),
	(8, 2353730135, 685, 'hologlitter'),
	(9, 2976881337, 690, 'technokitty'),
	(10, 3751481095, 692, 'phonytail'),
	(11, 3877296148, 693, 'haunted'),
	(12, 3965423840, 694, 'sparky'),
	(13, 3980353984, 695, 'bubblehead'),
	(14, 3986851258, 696, 'holonotes'),
	(15, 4220169270, 699, 'butterflies'),
	(16, 4289766731, 700, 'brainstorm');
/*!40000 ALTER TABLE `holoemote` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
