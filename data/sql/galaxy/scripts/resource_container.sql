# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-14 14:04:29
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy.resource_container
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
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.resource_container: ~0 rows (approximately)
DELETE FROM `resource_container`;
/*!40000 ALTER TABLE `resource_container` DISABLE KEYS */;
/*!40000 ALTER TABLE `resource_container` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
