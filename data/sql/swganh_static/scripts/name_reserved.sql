-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:18
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.name_reserved
DROP TABLE IF EXISTS `name_reserved`;
CREATE TABLE IF NOT EXISTS `name_reserved` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(32) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.name_reserved: ~9 rows (approximately)
/*!40000 ALTER TABLE `name_reserved` DISABLE KEYS */;
INSERT INTO `name_reserved` (`id`, `name`) VALUES
	(1, 'Luke'),
	(2, 'Darth'),
	(3, 'Vader'),
	(4, 'Leia'),
	(5, 'Chewbacca'),
	(6, 'Yoda'),
	(7, 'Emperor'),
	(8, 'stormtrooper'),
	(9, 'TK');
/*!40000 ALTER TABLE `name_reserved` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
