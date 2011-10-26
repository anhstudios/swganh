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

# Dumping structure for table galaxy.player_accounts_creatures
CREATE TABLE IF NOT EXISTS `player_accounts_creatures` (
  `player_id` bigint(20) NOT NULL,
  `creature_id` bigint(20) NOT NULL,
  PRIMARY KEY (`player_id`,`creature_id`),
  UNIQUE KEY `UNIQ_AFDAB341910BEE57` (`creature_id`),
  KEY `IDX_AFDAB34199E6F5DF` (`player_id`),
  CONSTRAINT `FK_AFDAB341910BEE57` FOREIGN KEY (`creature_id`) REFERENCES `creature` (`id`),
  CONSTRAINT `FK_AFDAB34199E6F5DF` FOREIGN KEY (`player_id`) REFERENCES `player_account` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.player_accounts_creatures: ~0 rows (approximately)
DELETE FROM `player_accounts_creatures`;
/*!40000 ALTER TABLE `player_accounts_creatures` DISABLE KEYS */;
/*!40000 ALTER TABLE `player_accounts_creatures` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
