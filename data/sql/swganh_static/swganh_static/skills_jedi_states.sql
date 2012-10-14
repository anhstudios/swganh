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

-- Dumping structure for table swganh_static.skills_jedi_states
DROP TABLE IF EXISTS `skills_jedi_states`;
CREATE TABLE IF NOT EXISTS `skills_jedi_states` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `name` char(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.skills_jedi_states: ~5 rows (approximately)
/*!40000 ALTER TABLE `skills_jedi_states` DISABLE KEYS */;
INSERT INTO `skills_jedi_states` (`id`, `name`) VALUES
	(0, 'none'),
	(1, 'forceSensitive'),
	(2, 'jedi'),
	(4, 'forceRankedLight'),
	(8, 'forceRankedDark');
/*!40000 ALTER TABLE `skills_jedi_states` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
