-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:00
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.player_accounts_creatures
DROP TABLE IF EXISTS `player_accounts_creatures`;
CREATE TABLE IF NOT EXISTS `player_accounts_creatures` (
  `player_id` bigint(20) NOT NULL,
  `creature_id` bigint(20) NOT NULL,
  PRIMARY KEY (`player_id`,`creature_id`),
  UNIQUE KEY `UNIQ_AFDAB341910BEE57` (`creature_id`),
  KEY `IDX_AFDAB34199E6F5DF` (`player_id`),
  CONSTRAINT `FK_AFDAB341910BEE57` FOREIGN KEY (`creature_id`) REFERENCES `creature` (`id`),
  CONSTRAINT `FK_AFDAB34199E6F5DF` FOREIGN KEY (`player_id`) REFERENCES `player_account` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.player_accounts_creatures: ~0 rows (approximately)
/*!40000 ALTER TABLE `player_accounts_creatures` DISABLE KEYS */;
/*!40000 ALTER TABLE `player_accounts_creatures` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
