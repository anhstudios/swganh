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

-- Dumping structure for table swganh_static.draft_slot_types
DROP TABLE IF EXISTS `draft_slot_types`;
CREATE TABLE IF NOT EXISTS `draft_slot_types` (
  `id` tinyint(3) unsigned NOT NULL AUTO_INCREMENT,
  `description` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.draft_slot_types: ~3 rows (approximately)
/*!40000 ALTER TABLE `draft_slot_types` DISABLE KEYS */;
INSERT INTO `draft_slot_types` (`id`, `description`) VALUES
	(2, 'Identical Component'),
	(4, 'Resource'),
	(5, 'Similar Components');
/*!40000 ALTER TABLE `draft_slot_types` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
