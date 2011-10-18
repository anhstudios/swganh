# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-16 19:39:50
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

use galaxy;

# Dumping structure for table galaxy.hopper_item
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
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.hopper_item: ~0 rows (approximately)
DELETE FROM `hopper_item`;
/*!40000 ALTER TABLE `hopper_item` DISABLE KEYS */;
/*!40000 ALTER TABLE `hopper_item` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
