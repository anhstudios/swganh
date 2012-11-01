-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:00
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.resource_container
DROP TABLE IF EXISTS `resource_container`;
CREATE TABLE IF NOT EXISTS `resource_container` (
  `id` bigint(20) NOT NULL,
  `resource_id` bigint(20) DEFAULT NULL,
  `current_quantity` int(11) NOT NULL,
  `max_quantity` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `IDX_6F6BBF8489329D25` (`resource_id`),
  CONSTRAINT `FK_6F6BBF8489329D25` FOREIGN KEY (`resource_id`) REFERENCES `resource` (`id`),
  CONSTRAINT `FK_6F6BBF84BF396750` FOREIGN KEY (`id`) REFERENCES `object` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.resource_container: ~0 rows (approximately)
/*!40000 ALTER TABLE `resource_container` DISABLE KEYS */;
/*!40000 ALTER TABLE `resource_container` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
