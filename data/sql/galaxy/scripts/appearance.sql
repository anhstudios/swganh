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

-- Dumping structure for table galaxy.appearance
DROP TABLE IF EXISTS `appearance`;
CREATE TABLE IF NOT EXISTS `appearance` (
  `object_id` bigint(20) NOT NULL,
  `scale` double NOT NULL,
  `gender` int(11) NOT NULL,
  `species` varchar(100) NOT NULL,
  `customization_data` longtext NOT NULL,
  PRIMARY KEY (`object_id`),
  CONSTRAINT `FK_AAB38290232D562B` FOREIGN KEY (`object_id`) REFERENCES `object` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Dumping data for table galaxy.appearance: ~0 rows (approximately)
/*!40000 ALTER TABLE `appearance` DISABLE KEYS */;
/*!40000 ALTER TABLE `appearance` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
