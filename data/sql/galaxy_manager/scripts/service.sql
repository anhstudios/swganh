# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-15 13:15:16
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy_manager.service
CREATE TABLE IF NOT EXISTS `service` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `galaxy_id` int(11) DEFAULT NULL,
  `name` varchar(255) NOT NULL,
  `type` varchar(255) NOT NULL,
  `address` bigint(20) NOT NULL,
  `tcp_port` int(11) NOT NULL,
  `udp_port` int(11) NOT NULL,
  `ping_port` int(11) NOT NULL,
  `version` int(11) NOT NULL,
  `status` int(11) NOT NULL,
  `last_pulse` decimal(17,3) NOT NULL,
  `created_at` datetime NOT NULL,
  `updated_at` datetime NOT NULL,
  PRIMARY KEY (`id`),
  KEY `IDX_E19D9AD2B61FAB2` (`galaxy_id`),
  CONSTRAINT `FK_E19D9AD2B61FAB2` FOREIGN KEY (`galaxy_id`) REFERENCES `galaxy` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=66 DEFAULT CHARSET=latin1;

# Dumping data for table galaxy_manager.service: ~3 rows (approximately)
DELETE FROM `service`;
/*!40000 ALTER TABLE `service` DISABLE KEYS */;
INSERT INTO `service` (`id`, `galaxy_id`, `name`, `type`, `address`, `tcp_port`, `udp_port`, `ping_port`, `version`, `status`, `last_pulse`, `created_at`, `updated_at`) VALUES
	(63, 1, 'ANH Character Service', 'character', 2130706433, 0, 0, 0, 0, 2, 20111015131516.677, '2011-10-15 13:12:30', '2011-10-15 13:12:30'),
	(64, 1, 'ANH Login Service', 'login', 2130706433, 0, 44453, 0, 0, 2, 20111015131516.677, '2011-10-15 13:12:30', '2011-10-15 13:12:30'),
	(65, 1, 'ANH Connection Service', 'connection', 2130706433, 0, 44463, 52685, 0, 2, 20111015131516.676, '2011-10-15 13:12:30', '2011-10-15 13:12:30');
/*!40000 ALTER TABLE `service` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
