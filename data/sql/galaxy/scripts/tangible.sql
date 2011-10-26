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

# Dumping structure for table galaxy.tangible
CREATE TABLE IF NOT EXISTS `tangible` (
  `id` bigint(20) NOT NULL,
  `customization` blob NOT NULL,
  `options_bitmask` int(11) NOT NULL,
  `incap_timer` int(11) NOT NULL,
  `condition_damage` int(11) NOT NULL,
  `max_condition` int(11) NOT NULL,
  `is_moveable` tinyint(1) NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `FK_80F8D49EBF396750` FOREIGN KEY (`id`) REFERENCES `object` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.tangible: ~16 rows (approximately)
DELETE FROM `tangible`;
/*!40000 ALTER TABLE `tangible` DISABLE KEYS */;
/*!40000 ALTER TABLE `tangible` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
