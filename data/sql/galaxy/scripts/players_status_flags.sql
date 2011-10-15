# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-15 14:37:15
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy.players_status_flags
CREATE TABLE IF NOT EXISTS `players_status_flags` (
  `player_id` bigint(20) NOT NULL,
  `status_flag_id` int(11) NOT NULL,
  PRIMARY KEY (`player_id`,`status_flag_id`),
  KEY `IDX_9475E7A299E6F5DF` (`player_id`),
  KEY `IDX_9475E7A2994F9360` (`status_flag_id`),
  CONSTRAINT `FK_9475E7A2994F9360` FOREIGN KEY (`status_flag_id`) REFERENCES `status_flag` (`id`),
  CONSTRAINT `FK_9475E7A299E6F5DF` FOREIGN KEY (`player_id`) REFERENCES `player` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.players_status_flags: ~0 rows (approximately)
DELETE FROM `players_status_flags`;
/*!40000 ALTER TABLE `players_status_flags` DISABLE KEYS */;
/*!40000 ALTER TABLE `players_status_flags` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
