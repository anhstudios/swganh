-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:58:57
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.hopper_item
DROP TABLE IF EXISTS `hopper_item`;
CREATE TABLE IF NOT EXISTS `hopper_item` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `installation_id` bigint(20) DEFAULT NULL,
  `resource_id` bigint(20) DEFAULT NULL,
  `quantity` double NOT NULL,
  PRIMARY KEY (`id`),
  KEY `IDX_E6F7A6B9167B88B4` (`installation_id`),
  KEY `IDX_E6F7A6B989329D25` (`resource_id`),
  CONSTRAINT `FK_E6F7A6B9167B88B4` FOREIGN KEY (`installation_id`) REFERENCES `installation` (`id`),
  CONSTRAINT `FK_E6F7A6B989329D25` FOREIGN KEY (`resource_id`) REFERENCES `resource` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.hopper_item: ~0 rows (approximately)
/*!40000 ALTER TABLE `hopper_item` DISABLE KEYS */;
/*!40000 ALTER TABLE `hopper_item` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
