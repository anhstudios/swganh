-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:16
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.datapad_types
DROP TABLE IF EXISTS `datapad_types`;
CREATE TABLE IF NOT EXISTS `datapad_types` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `object_string` varchar(255) NOT NULL DEFAULT 'object/tangible/datapad/shared_character_datapad.iff',
  `name` varchar(255) NOT NULL DEFAULT 'datapad',
  `file` varchar(255) NOT NULL DEFAULT 'item_n',
  PRIMARY KEY (`id`),
  UNIQUE KEY `datapad_object_string` (`object_string`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 ROW_FORMAT=REDUNDANT;

-- Dumping data for table swganh_static.datapad_types: ~7 rows (approximately)
/*!40000 ALTER TABLE `datapad_types` DISABLE KEYS */;
INSERT INTO `datapad_types` (`id`, `object_string`, `name`, `file`) VALUES
	(1, 'object/tangible/datapad/shared_character_datapad.iff', 'datapad', 'item_n'),
	(2, 'object/tangible/datapad/shared_droid_datapad_1.iff', 'datapad', 'item_n'),
	(3, 'object/tangible/datapad/shared_droid_datapad_2.iff', 'datapad', 'item_n'),
	(4, 'object/tangible/datapad/shared_droid_datapad_3.iff', 'datapad', 'item_n'),
	(5, 'object/tangible/datapad/shared_droid_datapad_4.iff', 'datapad', 'item_n'),
	(6, 'object/tangible/datapad/shared_droid_datapad_5.iff', 'datapad', 'item_n'),
	(7, 'object/tangible/datapad/shared_droid_datapad_6.iff', 'datapad', 'item_n');
/*!40000 ALTER TABLE `datapad_types` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
