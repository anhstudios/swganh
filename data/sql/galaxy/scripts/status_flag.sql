-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:01
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.status_flag
DROP TABLE IF EXISTS `status_flag`;
CREATE TABLE IF NOT EXISTS `status_flag` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `label` varchar(255) NOT NULL,
  `description` longtext NOT NULL,
  `flag` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.status_flag: ~0 rows (approximately)
/*!40000 ALTER TABLE `status_flag` DISABLE KEYS */;
/*!40000 ALTER TABLE `status_flag` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
