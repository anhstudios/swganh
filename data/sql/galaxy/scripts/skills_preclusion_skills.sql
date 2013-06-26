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

-- Dumping structure for table galaxy.skills_preclusion_skills
DROP TABLE IF EXISTS `skills_preclusion_skills`;
CREATE TABLE IF NOT EXISTS `skills_preclusion_skills` (
  `skill_id` int(11) NOT NULL,
  `preclusion_skill_id` int(11) NOT NULL,
  PRIMARY KEY (`skill_id`,`preclusion_skill_id`),
  KEY `IDX_F89ED6FB5585C142` (`skill_id`),
  KEY `IDX_F89ED6FBB334A428` (`preclusion_skill_id`),
  CONSTRAINT `FK_F89ED6FB5585C142` FOREIGN KEY (`skill_id`) REFERENCES `skill` (`id`),
  CONSTRAINT `FK_F89ED6FBB334A428` FOREIGN KEY (`preclusion_skill_id`) REFERENCES `skill` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.skills_preclusion_skills: ~8 rows (approximately)
/*!40000 ALTER TABLE `skills_preclusion_skills` DISABLE KEYS */;
INSERT INTO `skills_preclusion_skills` (`skill_id`, `preclusion_skill_id`) VALUES
	(707, 745),
	(745, 707),
	(1012, 1031),
	(1012, 1050),
	(1031, 1012),
	(1031, 1050),
	(1050, 1012),
	(1050, 1031);
/*!40000 ALTER TABLE `skills_preclusion_skills` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
