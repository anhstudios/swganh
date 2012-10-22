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

-- Dumping structure for table swganh_static.csr_categories
DROP TABLE IF EXISTS `csr_categories`;
CREATE TABLE IF NOT EXISTS `csr_categories` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'CSR Category ID',
  `name` varchar(45) NOT NULL DEFAULT '""' COMMENT 'CSR Category Name',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=32 DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.csr_categories: ~31 rows (approximately)
/*!40000 ALTER TABLE `csr_categories` DISABLE KEYS */;
INSERT INTO `csr_categories` (`id`, `name`) VALUES
	(1, 'Bazaar'),
	(2, 'Badges'),
	(3, 'Bank'),
	(4, 'Chat'),
	(5, 'Combat'),
	(6, 'Crafting'),
	(7, 'Dancing'),
	(8, 'Datapad'),
	(9, 'Elevators'),
	(10, 'Faction'),
	(11, 'Group'),
	(12, 'Guild'),
	(13, 'Harvesting'),
	(14, 'Inventory'),
	(15, 'Item'),
	(16, 'Looting'),
	(17, 'Misc'),
	(18, 'Music'),
	(19, 'NPC'),
	(20, 'Profession'),
	(21, 'Sampling'),
	(22, 'Server Administration'),
	(23, 'Structures'),
	(24, 'Survey'),
	(25, 'System Messages'),
	(26, 'Trading'),
	(27, 'Trainers'),
	(28, 'Travel'),
	(29, 'UI'),
	(30, 'Waypoints'),
	(31, 'Weather');
/*!40000 ALTER TABLE `csr_categories` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
