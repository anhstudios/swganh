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

-- Dumping structure for table galaxy.creatures_skills
DROP TABLE IF EXISTS `creatures_skills`;
CREATE TABLE IF NOT EXISTS `creatures_skills` (
  `creature_id` bigint(20) NOT NULL,
  `skill_id` int(11) NOT NULL,
  PRIMARY KEY (`creature_id`,`skill_id`),
  KEY `IDX_56339608F9AB048` (`creature_id`),
  KEY `IDX_563396085585C142` (`skill_id`),
  CONSTRAINT `FK_563396085585C142` FOREIGN KEY (`skill_id`) REFERENCES `skill` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK_56339608F9AB048` FOREIGN KEY (`creature_id`) REFERENCES `creature` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.creatures_skills: ~0 rows (approximately)
/*!40000 ALTER TABLE `creatures_skills` DISABLE KEYS */;
/*!40000 ALTER TABLE `creatures_skills` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
