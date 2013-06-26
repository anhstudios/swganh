-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:58:57
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.creatures_xp
DROP TABLE IF EXISTS `creatures_xp`;
CREATE TABLE IF NOT EXISTS `creatures_xp` (
  `creature_id` bigint(20) NOT NULL,
  `xp_type_id` int(11) NOT NULL,
  `value` int(10) NOT NULL DEFAULT '0',
  PRIMARY KEY (`creature_id`,`xp_type_id`),
  KEY `fk_charxp_xp_xp` (`xp_type_id`),
  CONSTRAINT `character_xp_ibfk_2` FOREIGN KEY (`creature_id`) REFERENCES `creature` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_charxp_xp_xp` FOREIGN KEY (`xp_type_id`) REFERENCES `xp_type` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.creatures_xp: ~0 rows (approximately)
/*!40000 ALTER TABLE `creatures_xp` DISABLE KEYS */;
/*!40000 ALTER TABLE `creatures_xp` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
