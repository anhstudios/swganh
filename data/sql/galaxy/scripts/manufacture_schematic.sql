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

# Dumping structure for table galaxy.manufacture_schematic
CREATE TABLE IF NOT EXISTS `manufacture_schematic` (
  `id` bigint(20) NOT NULL,
  `quantity` int(11) NOT NULL,
  `creator` varchar(255) NOT NULL,
  `data_size` double NOT NULL,
  `customization` varchar(255) NOT NULL,
  `customization_model` varchar(255) NOT NULL,
  `prototype_model` varchar(255) NOT NULL,
  `is_active` tinyint(1) NOT NULL,
  `risk_factor` double NOT NULL,
  `is_ready` tinyint(1) NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `FK_51DC8E01BF396750` FOREIGN KEY (`id`) REFERENCES `object` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.manufacture_schematic: ~0 rows (approximately)
DELETE FROM `manufacture_schematic`;
/*!40000 ALTER TABLE `manufacture_schematic` DISABLE KEYS */;
/*!40000 ALTER TABLE `manufacture_schematic` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
