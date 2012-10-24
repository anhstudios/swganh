-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:19
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.skills_preclusions
DROP TABLE IF EXISTS `skills_preclusions`;
CREATE TABLE IF NOT EXISTS `skills_preclusions` (
  `skill_id` int(11) unsigned NOT NULL,
  `prec_skill_id` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`skill_id`,`prec_skill_id`),
  KEY `fk_skills_prec_prec_prec` (`prec_skill_id`),
  CONSTRAINT `fk_skills_prec_prec_prec` FOREIGN KEY (`prec_skill_id`) REFERENCES `skills` (`skill_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_skill_prec_skill_skill` FOREIGN KEY (`skill_id`) REFERENCES `skills` (`skill_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.skills_preclusions: ~8 rows (approximately)
/*!40000 ALTER TABLE `skills_preclusions` DISABLE KEYS */;
INSERT INTO `skills_preclusions` (`skill_id`, `prec_skill_id`) VALUES
	(745, 707),
	(707, 745),
	(1031, 1012),
	(1050, 1012),
	(1012, 1031),
	(1050, 1031),
	(1012, 1050),
	(1031, 1050);
/*!40000 ALTER TABLE `skills_preclusions` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
