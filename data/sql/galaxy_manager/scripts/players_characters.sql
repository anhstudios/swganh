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

# Dumping structure for table galaxy_manager.players_characters
CREATE TABLE IF NOT EXISTS `players_characters` (
  `player_id` bigint(20) NOT NULL,
  `character_id` bigint(20) NOT NULL,
  PRIMARY KEY (`player_id`,`character_id`),
  UNIQUE KEY `UNIQ_1C6ECE9A1136BE75` (`character_id`),
  KEY `IDX_1C6ECE9A99E6F5DF` (`player_id`),
  CONSTRAINT `players_characters_ibfk_1` FOREIGN KEY (`player_id`) REFERENCES `player` (`id`),
  CONSTRAINT `players_characters_ibfk_2` FOREIGN KEY (`character_id`) REFERENCES `characters` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

# Dumping data for table galaxy_manager.players_characters: ~13 rows (approximately)
DELETE FROM `players_characters`;
/*!40000 ALTER TABLE `players_characters` DISABLE KEYS */;
INSERT INTO `players_characters` (`player_id`, `character_id`) VALUES
	(1, 1),
	(1, 2),
	(1, 12),
	(1, 16),
	(1, 17),
	(1, 18),
	(1, 19),
	(1, 20),
	(1, 21),
	(1, 22),
	(1, 23),
	(1, 24),
	(1, 25);
/*!40000 ALTER TABLE `players_characters` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
