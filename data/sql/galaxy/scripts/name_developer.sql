-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:58:59
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.name_developer
DROP TABLE IF EXISTS `name_developer`;
CREATE TABLE IF NOT EXISTS `name_developer` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.name_developer: ~20 rows (approximately)
/*!40000 ALTER TABLE `name_developer` DISABLE KEYS */;
INSERT INTO `name_developer` (`id`, `name`) VALUES
	(1, 'snow'),
	(2, 'htx'),
	(3, 'wookiee'),
	(4, 'jack'),
	(5, 'rouse'),
	(6, 'ketusringa'),
	(7, 'yosh'),
	(8, 'jay'),
	(9, 'schmunzel'),
	(10, 'ready'),
	(11, 'spety'),
	(12, 'narook'),
	(13, 'tmr'),
	(14, 'o_0'),
	(15, 'starko'),
	(16, 'cieto'),
	(18, 'pope'),
	(19, 'apathy'),
	(20, 'deadlock'),
	(21, 'kronos');
/*!40000 ALTER TABLE `name_developer` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
