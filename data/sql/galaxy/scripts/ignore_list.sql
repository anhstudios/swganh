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

# Dumping structure for table galaxy.ignore_list
CREATE TABLE IF NOT EXISTS `ignore_list` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `player_id` bigint(20) DEFAULT NULL,
  `ignored_player_id` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `IDX_124ECE8C99E6F5DF` (`player_id`),
  KEY `IDX_124ECE8CD7A43A0C` (`ignored_player_id`),
  CONSTRAINT `FK_124ECE8C99E6F5DF` FOREIGN KEY (`player_id`) REFERENCES `player` (`id`),
  CONSTRAINT `FK_124ECE8CD7A43A0C` FOREIGN KEY (`ignored_player_id`) REFERENCES `player` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.ignore_list: ~0 rows (approximately)
DELETE FROM `ignore_list`;
/*!40000 ALTER TABLE `ignore_list` DISABLE KEYS */;
/*!40000 ALTER TABLE `ignore_list` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
