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

-- Dumping structure for table swganh_static.badge_types
DROP TABLE IF EXISTS `badge_types`;
CREATE TABLE IF NOT EXISTS `badge_types` (
  `id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Badge Type ID',
  `name` varchar(255) DEFAULT NULL COMMENT 'Badge Type',
  PRIMARY KEY (`id`),
  UNIQUE KEY `pk_name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=REDUNDANT;

-- Dumping data for table swganh_static.badge_types: ~8 rows (approximately)
/*!40000 ALTER TABLE `badge_types` DISABLE KEYS */;
INSERT INTO `badge_types` (`id`, `name`) VALUES
	(1, 'accumulation'),
	(7, 'content'),
	(6, 'events'),
	(4, 'exploration_dangerous'),
	(5, 'exploration_easy'),
	(3, 'exploration_jedi'),
	(2, 'interest'),
	(8, 'master');
/*!40000 ALTER TABLE `badge_types` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
