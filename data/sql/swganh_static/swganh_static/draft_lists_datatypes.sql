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

-- Dumping structure for table swganh_static.draft_lists_datatypes
DROP TABLE IF EXISTS `draft_lists_datatypes`;
CREATE TABLE IF NOT EXISTS `draft_lists_datatypes` (
  `id` tinyint(3) unsigned NOT NULL AUTO_INCREMENT,
  `description` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.draft_lists_datatypes: ~12 rows (approximately)
/*!40000 ALTER TABLE `draft_lists_datatypes` DISABLE KEYS */;
INSERT INTO `draft_lists_datatypes` (`id`, `description`) VALUES
	(0, 'Potency'),
	(1, 'Cold Resistance'),
	(2, 'Conductivity'),
	(3, 'Decay Resistance'),
	(4, 'Heat Resistance'),
	(5, 'Flavor'),
	(6, 'Mallebility'),
	(7, 'Potential Energy'),
	(8, 'Overall Quality'),
	(9, 'Shock Resistance'),
	(10, 'Unit Toughness'),
	(11, 'Bulk');
/*!40000 ALTER TABLE `draft_lists_datatypes` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
