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

-- Dumping structure for table galaxy.container_permissions
DROP TABLE IF EXISTS `container_permissions`;
CREATE TABLE IF NOT EXISTS `container_permissions` (
  `type` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(35) DEFAULT NULL,
  PRIMARY KEY (`type`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.container_permissions: ~7 rows (approximately)
/*!40000 ALTER TABLE `container_permissions` DISABLE KEYS */;
INSERT INTO `container_permissions` (`type`, `name`) VALUES
	(1, 'default'),
	(2, 'world'),
	(3, 'static'),
	(4, 'world cell'),
	(5, 'creature'),
	(6, 'creature container'),
	(7, 'rideable'),
	(8, 'no view');
/*!40000 ALTER TABLE `container_permissions` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
