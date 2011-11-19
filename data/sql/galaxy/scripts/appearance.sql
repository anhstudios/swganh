# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-21 22:52:08
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy.appearance
CREATE TABLE IF NOT EXISTS `appearance` (
  `object_id` bigint(20) NOT NULL,
  `scale` double NOT NULL,
  `gender` int(11) NOT NULL,
  `species` varchar(100) NOT NULL,
  `customization_data` longtext NOT NULL,
  PRIMARY KEY (`object_id`),
  CONSTRAINT `FK_AAB38290232D562B` FOREIGN KEY (`object_id`) REFERENCES `object` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.appearance: ~16 rows (approximately)
DELETE FROM `appearance`;
/*!40000 ALTER TABLE `appearance` DISABLE KEYS */;
/*!40000 ALTER TABLE `appearance` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
