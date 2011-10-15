# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-15 13:18:33
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy.tangibles_defenders
CREATE TABLE IF NOT EXISTS `tangibles_defenders` (
  `tangible_id` bigint(20) NOT NULL,
  PRIMARY KEY (`tangible_id`),
  KEY `IDX_C6524BE2C7FE2A7F` (`tangible_id`),
  CONSTRAINT `FK_C6524BE2C7FE2A7F` FOREIGN KEY (`tangible_id`) REFERENCES `tangible` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.tangibles_defenders: ~0 rows (approximately)
DELETE FROM `tangibles_defenders`;
/*!40000 ALTER TABLE `tangibles_defenders` DISABLE KEYS */;
/*!40000 ALTER TABLE `tangibles_defenders` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
